# MPG Handwheel Prototype Using ESP32 and Modbus TCP library
To connect the ESP32 to the network (only has to be done once):

1. Edit the sketch with an ip address, gateway and subnet mask that matches the network that the device will connect to
2. Upload the sketch to the device
3. From the computer, connect to the wireless network named AutoConnectAP
4. Enter the network SSID and password in the wifi portal that comes up. If the wifi portal does not automatically launch then enter "192.168.4.1" into the address bar of any      internet browser. This should bring up the wifi portal page. 

## Parts List (Custom MPG)
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

## Parts List (Amazon MPG)
1.	https://www.amazon.com/gp/product/B01E6VTZHC/ref=ppx_yo_dt_b_asin_title_o07_s00?ie=UTF8&psc=1
2.	https://www.amazon.com/gp/product/B07BTRLKPT/ref=ppx_yo_dt_b_asin_title_o04_s04?ie=UTF8&psc=1
3.	https://www.amazon.com/gp/product/B082XJXQS6/ref=ppx_yo_dt_b_asin_title_o04_s00?ie=UTF8&psc=1
4.	https://www.amazon.com/gp/product/B07XG323G8/ref=ppx_yo_dt_b_asin_title_o03_s01?ie=UTF8&psc=1
5.	https://www.amazon.com/gp/product/B07T4YHP7Q/ref=ppx_yo_dt_b_asin_title_o02_s00?ie=UTF8&psc=1
6.	https://www.amazon.com/gp/product/B07Q576VWZ/ref=ppx_yo_dt_b_asin_title_o04_s03?ie=UTF8&psc=1

## Wiring for Amazon Handwheel
battery -> TP4056 Charger Module -> XL6009 Boost Convert (3.3 to 5V) -> MPG Estop switch -> ESP32 VIN/GND <br> <br>
(The LED on the MPG PCB is connected to the 3.3v and GND terminals on the ESP32) <br> <br>
MPG PCB: <br>
    ESP32 GND    -> COM <br>
    ESP32 Pin 2  -> A+ <br>
    ESP32 Pin 4  -> B+ <br>
    ESP32 Pin 13 -> X Axis <br>
    ESP32 Pin 14 -> Y Axis <br>
    ESP32 Pin 27 -> Z Axis <br>
    ESP32 Pin 25 -> MPG Enable Switch <br>
    ESP32 Pin 32 -> A Axis <br>
    ESP32 Pin 35 -> B Axis (8k ohm pullup to 3.3v) <br>
    ESP32 Pin 34 -> C Axis (8k ohm pullup to 3.3v) <br>
    ESP32 Pin 15 -> Inc 1 <br>
    ESP32 Pin 21 -> Inc 2 <br>
    ESP32 Pin 22 -> Inc 3 <br> <br>

(only if you want ethernet instead of just WiFi) <br>
W5500 Lite: <br>
    ESP32 Pin 23 -> MOSI <br>
    ESP32 Pin 19 -> MISO <br>
    ESP32 Pin 18 -> SCLK <br>
    ESP32 Pin 5  -> CS <br>
    ESP32 Pin 26 -> RST <br>

## Demonstration
https://www.youtube.com/watch?v=6ejczGxR-vw <br>
https://www.youtube.com/watch?v=FvmKIaZbhRo

## More info
https://www.machsupport.com/forum/index.php?topic=43759.msg281820#msg281820

## Required Libraries
https://github.com/emelianov/modbus-esp8266 <br>
https://github.com/madhephaestus/ESP32Encoder <br>
https://github.com/tzapu/WiFiManager


