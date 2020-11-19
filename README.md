# MPG Handwheel Prototype Using ESP32 and Modbus TCP library
To connect the ESP32 to the network (only has to be done once):

1. Edit the sketch with an ip address, gateway and subnet mask that matches the network that the device will connect to
2. Upload the sketch to the device
3. From the computer, connect to the wireless network named AutoConnectAP
4. Enter the network SSID and password in the wifi portal that comes up. If the wifi portal does not automatically launch then enter "192.168.4.1" into the address bar of any      internet browser. This should bring up the wifi portal page. 

## Parts List
1.	https://www.amazon.com/gp/product/B01E6VTZHC/ref=ppx_yo_dt_b_asin_title_o07_s00?ie=UTF8&psc=1
2.	https://www.amazon.com/gp/product/B07BTRLKPT/ref=ppx_yo_dt_b_asin_title_o04_s04?ie=UTF8&psc=1
3.	https://www.amazon.com/gp/product/B082XJXQS6/ref=ppx_yo_dt_b_asin_title_o04_s00?ie=UTF8&psc=1
4.	https://www.amazon.com/gp/product/B07XG323G8/ref=ppx_yo_dt_b_asin_title_o03_s01?ie=UTF8&psc=1
5.	https://www.amazon.com/gp/product/B07Q576VWZ/ref=ppx_yo_dt_b_asin_title_o04_s03?ie=UTF8&psc=1

## Wiring
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

## Demonstration
https://www.youtube.com/watch?v=FvmKIaZbhRo

## More info
https://www.machsupport.com/forum/index.php?topic=43759.msg281820#msg281820

## Required Libraries
https://github.com/emelianov/modbus-esp8266 <br>
https://github.com/madhephaestus/ESP32Encoder <br>
https://github.com/tzapu/WiFiManager


