#Low Cost Home Automation
##Needed:
###Hardware
 - ESP8266 (-1 or -12) (about 2$ from China)
###Software
 - Arduino IDE
  -Adafruit MQTT Library
   -via "Import Library"
  -ESP8266 Firmware
   -Download from https://github.com/esp8266/Arduino
 - Mosquitto MQTT Broker
  - see http://mosquitto.org/ and https://github.com/wtommy/mosquitto
 - Home-Assistant []
  - see https://home-assistant.io and https://github.com/balloob/home-assistant

A Server to run the Software is also needed, for example RPi 2 or Odroid C1+ ( http://odroid.com/dokuwiki/doku.php?id=en:odroid-c1)
I recommend running Arch Linux on it.

 ####Install Mosquitto via Yaourt:
 "yaourt -S mosquitto"
 "sudo pacman-key -r 779B22DFB3E717B7"
/*
 When You get the following Error:
 "gpg: connecting dirmngr at '/root/.gnupg/S.dirmngr' failed: IPC "connect" "
Execute: (from https://wiki.archlinux.org/index.php/Pacman/Package_signing)
 "sudo dirmngr < /dev/null"
Update Key-Chain:
"sudo pacman-key -u"
*/
###configurating Mosquitto:
You can configure Mosquitto with a .conf file.
Important is that you allow connecting to the server from everywhere in your network. If you set the Server up in your local home network you do not have to worry about security that much because your router should not allow accessing your local network.
--> I had a problem with "listen 1883 127.0.0.1" because this only allowed access from within the server. Took 4 Hours to fix. :(
###Configurating Home-Assistant:
###Arduino Software:
####SimpleTimer
>This is (yet another) simple library to launch timed actions.
>It's based on millis(), thus it has 1 ms resolution.
>http://playground.arduino.cc/Code/SimpleTimer
Used to Push Data in certain Interval.
First Tried Ticker (https://github.com/esp8266/Arduino/tree/ea302aab05480ad36c076b618abb642b1eb1893b/hardware/esp8266com/esp8266/libraries/Ticker ) but it uses interupts, which did not work.
We do not need exact timing for Pushing the Sensor Data, so I ditched it for the SimpleTimer Lib. This Lib should be alot safer and less trouble.
####Hardware-Setups:
#####Reed-Sensor:
2aa

####Preparing & Flashing the Devices
Client Scripts for:
 [] Temperature-Sensor DS18S20 (http://www.pollin.de/shop/dt/NTg5OTE4OTk-/Bauelemente_Bauteile/Aktive_Bauelemente/Sensoren_Peltier_Elemente/Temperatursensor_DS18S20.html)
  [] Temperature-Sensor DHT22
  [] Reed-Sensor
  [] Light-Sensor (LDR or maybe photodiode)
  [] LED-Strip Module

####Installing Devices & first startup:
 - When you Power up a Device it creates a Wifi-Hotspot called ESP8266.
 - Connect to the Hotspot using the Password: Home-Control
 - Navigate to Page "*ESP IP*/index.html"
 - Enter Wifi Name & Password
 - Enter MQTT-Server IP (e.g 192.168.2.111)
 - Enter Room Identifier (e.g bedroom)
 - Enter Sensor Identifier (e.g balcony_door)

 ----
##Selfmade - Hardware:
###LED-Strip Module
 To replace sample drivers.
 Input: 12V DC
 Output:  3x 0-12V DC (RGB)
 Schematic example:
12V --> 3.3V Regulator -> ESP8266 -> 3 MosFET driver -> 3x 0-12V Output
ESP8266 should be configurable via Wifi
###Sensors:
Sensors need to be ultra-low-power.
The ESP8266 Wifi might need to much power and too long to power up when used with sensors.
So I will evaluate the 2.4 GHz NRF24L01+ Radio Transceiver. I still consider using the esp8266 as MCU just disabling WiFi most of the times
####Links
2.4 GHz NRF24L01+ Datasheet: http://www.nordicsemi.com/eng/content/download/2726/34069/file/nRF24L01P_Product_Specification_1_0.pdf

Maybe use libraries of mysensors, see:
https://github.com/mysensors/Arduino/tree/master/libraries
They have a Library for The Dallas Temperature-Sensor already.
They work using a Library for Communicating to other nodes via the NRF24L01. They even support Firmware-Updates!
They are supported by Home-Assistant as well. https://home-assistant.io/components/sensor.mysensors.html


Some useful Info for NRF24L01
https://maniacbug.wordpress.com/2011/10/19/sensor-node/

##Discussion:
