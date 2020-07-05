# MPG Handwheel Prototype Using ESP32 and Modbus TCP library
To connect the ESP32 to the network (only has to be done once):

1. Edit the sketch with an ip address, gateway and subnet mask that matches the network that the device will connect to
2. Upload the sketch to the device
3. From the computer, connect to the wireless network named AutoConnectAP
4. Enter the network SSID and password in the wifi portal that comes up. If the wifi portal does not automatically launch then enter "192.168.4.1" into the address bar of any      internet browser. This should bring up the wifi portal page. 

## Demonstration
https://www.youtube.com/watch?v=AQGv1RTwftI

## Required Libraries
https://github.com/emelianov/modbus-esp8266 <br>
https://github.com/madhephaestus/ESP32Encoder <br>
https://github.com/tzapu/WiFiManager


