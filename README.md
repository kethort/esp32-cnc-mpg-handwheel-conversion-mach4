# MPG Handwheel Prototype Using ESP32 and Modbus TCP library
To connect the ESP32 to the network (only has to be done once):

1. Edit the sketch with an ip address, gateway and subnet mask that matches the network that the device will connect to
2. Upload the sketch to the device
3. From the computer, connect to the wireless network named AutoConnectAP
4. Enter the network SSID and password in the wifi portal that comes up. If the wifi portal does not automatically launch then enter "192.168.4.1" into the address bar of any      internet browser. This should bring up the wifi portal page. 

## Parts List
1. 2x 12-Pin 1-Pole 11-Position Rotary Switch
2. 1x Momentary Switch
3. 1x 3.7v 2800mAh Lipo Battery
4. 1x 5v 4-Terminal 100ppr Manual Pulse Generator
5. 1x ESP-32S Development Board
6. 1x TP4056 Lithium Battery Charger Module
7. 1x SPST Switch
8. (optional) 1x 10uf Electrolytic Capacitor [any voltage]

## Wiring
<p align="center">
  <img src="/img/WirelessMPG_bb.png" width="750"/>
</p>

## Demonstration
https://www.youtube.com/watch?v=6ejczGxR-vw

## Required Libraries
https://github.com/emelianov/modbus-esp8266 <br>
https://github.com/madhephaestus/ESP32Encoder <br>
https://github.com/tzapu/WiFiManager


