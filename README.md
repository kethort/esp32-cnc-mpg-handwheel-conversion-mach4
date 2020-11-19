# MPG Handwheel Prototype Using ESP32 and Modbus TCP library

## Required Arduino Libraries
https://github.com/emelianov/modbus-esp8266 <br>
https://github.com/madhephaestus/ESP32Encoder <br>
https://github.com/tzapu/WiFiManager

## ESP32 Firmware and Network Setup
1. Upload the Mach4MPGModbus.ino file to the ESP32 using the Arduino (or other) IDE.
2. Reboot the ESP32.
3. The ESP32 device should show up in the list of network access points. 

<p align="center">
  <img src="/img/access_point.png" width="250"/>
</p>

4. Connect to this access point and navigate to 192.168.4.1 in a browser. 
5. Press the "Configure Wifi" button.

<p align="center">
  <img src="/img/captive_portal.png" width="300"/>
</p>

6. Find your network access point and connect to it by providing your network credentials.

<p align="center">
  <img src="/img/captive_portal2.PNG" width="250"/>
</p>

## Mach4 Software Setup
1. Launch the MPGInstaller.exe from the dist folder of the repo.
2. Find the local IP address of the ESP32 (espressif). 

<p align="center">
  <img src="/img/ip_address.PNG" width="300"/>
</p>

3. Select the Mach4 profile you want to update, enter the IP address of the ESP32 and press install. 

<p align="center">
  <img src="/img/mpg_installer.PNG" width="300"/>
</p>

4. Turn on the ESP32 MPG, open Mach4 and navigate to Diagnostic->Modbus in the File Menu.
5. To connect or reconnect to the modbus device, press the stop button in the diagnostic window and then the play button. 

<p align="center">
  <img src="/img/modbus_diag.PNG" width="300"/>
</p>
 
## Parts List
1.  https://www.amazon.com/gp/product/B01E6VTZHC/ref=ppx_yo_dt_b_asin_title_o07_s00?ie=UTF8&psc=1
2.  https://www.amazon.com/gp/product/B07BTRLKPT/ref=ppx_yo_dt_b_asin_title_o04_s04?ie=UTF8&psc=1
3.  https://www.amazon.com/gp/product/B082XJXQS6/ref=ppx_yo_dt_b_asin_title_o04_s00?ie=UTF8&psc=1
4.  https://www.amazon.com/gp/product/B07XG323G8/ref=ppx_yo_dt_b_asin_title_o03_s01?ie=UTF8&psc=1
5.  https://www.amazon.com/gp/product/B07Q576VWZ/ref=ppx_yo_dt_b_asin_title_o04_s03?ie=UTF8&psc=1

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
