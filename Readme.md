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
