/***************************************************
  Adafruit MQTT Library ESP8266 Example

  Must use ESP8266 Arduino from:
    https://github.com/esp8266/Arduino

  Works great with Adafruit's Huzzah ESP board:
  ----> https://www.adafruit.com/product/2471

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Tony DiCola for Adafruit Industries.
  MIT license, all text above must be included in any redistribution
 ****************************************************/
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
/************************* SENSOR CONFIGURATION ******************************/

#define ONE_WIRE_BUS 13
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress insideThermometer;

/************************* WiFi Access Point *********************************/

#define WLAN_SSID       "Li88KBo"
#define WLAN_PASS       "FotografierenMachtSpass"

/************************* Adafruit.io Setup *********************************/

#define AIO_SERVER      "192.168.2.111"
#define AIO_SERVERPORT  1883
#define AIO_USERNAME    "mirk"
#define AIO_KEY         "9HDxzigxBWu79HDxzigxBWu7B7QQVA8o"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;

// Store the MQTT server, client ID, username, and password in flash memory.
// This is required for using the Adafruit MQTT library.
const char MQTT_SERVER[] PROGMEM    = AIO_SERVER;
// Set a unique MQTT client ID using the AIO key + the date and time the sketch
// was compiled (so this should be unique across multiple devices for a user,
// alternatively you can manually set this to a GUID or other random value).
const char MQTT_CLIENTID[] PROGMEM  = __TIME__ AIO_USERNAME;
const char MQTT_USERNAME[] PROGMEM  = AIO_USERNAME;
const char MQTT_PASSWORD[] PROGMEM  = AIO_KEY;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, MQTT_SERVER, AIO_SERVERPORT, MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD);

/****************************** Feeds ***************************************/

// Setup a feed called 'photocell' for publishing.
// Notice MQTT paths for AIO follow the form: <username>/feeds/<feedname>
const char PHOTOCELL_FEED[] PROGMEM = "home/bedroom/temperature";
Adafruit_MQTT_Publish photocell = Adafruit_MQTT_Publish(&mqtt, PHOTOCELL_FEED);

// Setup a feed called 'onoff' for subscribing to changes.
const char ONOFF_CMD_FEED[] PROGMEM = "home/bedroom/switch1/set";
Adafruit_MQTT_Subscribe onoff_cmd = Adafruit_MQTT_Subscribe(&mqtt, ONOFF_CMD_FEED);


const char ONOFF_STATUS_FEED[] PROGMEM = "home/bedroom/switch1";
Adafruit_MQTT_Publish onoff_status = Adafruit_MQTT_Publish(&mqtt, ONOFF_STATUS_FEED);

/*************************** Sketch Code ************************************/

char *switch_status = "ON";
const int SWITCHPIN = 15;
const int data_interval = 5000; // in ms
unsigned long previousMillis = 0;    

void setup() {
  pinMode(SWITCHPIN, OUTPUT);
  // Start Sensor
  sensors.begin();
  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0"); 
  sensors.setResolution(insideThermometer, 9);
 
  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC); 
  // END SENSOR
  
  Serial.begin(115200);
  delay(10);

  Serial.println(F("Adafruit MQTT demo"));

  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WLAN_SSID);

  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for onoff feed.
  mqtt.subscribe(&onoff_cmd);

  //Discard first Temp 
  
  sensors.requestTemperatures(); // Send the command to get temperatures
  sensors.getTempC(insideThermometer);
  delay(500);
}

uint32_t x=0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // Try to ping the MQTT server7:36:15m
  /*
  if (! mqtt.ping(3) ) {
    // MQTT pings failed, lets reconnect
    Serial.println("Ping fail!");
  }
  */

  // this is our 'wait for incoming subscription packets' busy subloop

  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(1000))) {
    Serial.print("Read Subs");
    if (subscription == &onoff_cmd) {
      char *ret = (char *)onoff_cmd.lastread;
      Serial.print(ret);
      
      if(strcmp(ret,"ON") == 0){
        Serial.print("Switch HIGH");
        switch_status = "ON";
        digitalWrite(SWITCHPIN, HIGH);
      }
      if(strcmp(ret,"OFF") == 0){
        Serial.print("Switch LOW");
        switch_status = "OFF";
        digitalWrite(SWITCHPIN, LOW);
      }
    }
  }
  
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= data_interval) {
    previousMillis = currentMillis;
    // Now we can publish stuff!
    Serial.print("Requesting temperatures...");
    sensors.requestTemperatures(); // Send the command to get temperatures
    Serial.println("DONE");
    
    Serial.print(F("\nSending Temperature val "));
    x = sensors.getTempC(insideThermometer);
    onoff_status.publish(switch_status);
    Serial.print(x);
    Serial.print("...");
    if (! photocell.publish(x)) {
      Serial.println(F("Failed"));
    } else {
      Serial.println(F("OK!"));
    }
  }
}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
  }
  Serial.println("MQTT Connected!");
}
