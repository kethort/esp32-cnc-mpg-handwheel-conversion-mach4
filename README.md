# Mach4 Wireless (Wifi) MPG Handwheel Using ESP32 and Modbus TCP library

<p align="center">
  <img src="/img/mpg_esp32.jpg" width="700"/>
</p>

## Demonstration
https://www.youtube.com/watch?v=FvmKIaZbhRo

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
1. Find the local IP address of the ESP32 (espressif). 

<p align="center">
  <img src="/img/ip_address.PNG" width="300"/>
</p>

2. Launch the MPGInstaller.exe from the dist folder of the repo.
3. Select the Mach4 profile you want to update, enter the IP address of the ESP32 and press install. (if the ESP32 device is already connected to the network, the IP address should auto-populate)

<p align="center">
  <img src="/img/mpg_installer.PNG" width="300"/>
</p>

4. Turn on the ESP32 MPG, open Mach4 and navigate to Diagnostic->Modbus in the File Menu.
5. To connect or reconnect to the modbus device, press the stop button in the diagnostic window and then the play button. <i>(Alternatively use the Wizard instead [step 6])</i>

<p align="center">
  <img src="/img/modbus_diag.PNG" width="300"/>
</p>

6. The wizard can be accessed by opening the Wizard launcher in the Mach4 file menu. (Wizard->Select Wizard->MPGWiz)
   
<p align="center">
  <img src="/img/wizard.PNG" width="300"/>
</p>
 
## Parts List
1.  [MPG Handwheel(]https://www.amazon.com/gp/product/B01E6VTZHC/ref=ppx_yo_dt_b_asin_title_o07_s00?ie=UTF8&psc=1)
2.  [Lipo Battery(]https://www.amazon.com/gp/product/B07BTRLKPT/ref=ppx_yo_dt_b_asin_title_o04_s04?ie=UTF8&psc=1)
3.  [Lithium Battery Charger Module](https://www.amazon.com/gp/product/B082XJXQS6/ref=ppx_yo_dt_b_asin_title_o04_s00?ie=UTF8&psc=1)
4.  [Boost Converter](https://www.amazon.com/gp/product/B07XG323G8/ref=ppx_yo_dt_b_asin_title_o03_s01?ie=UTF8&psc=1)
5.  [ESP32](https://www.amazon.com/Xiuxin-Development-Dual-Mode-Bluetooth-Antenna/dp/B079PVCF2G/ref=sxts_sxwds-bia-wc-rsf-lq2a1_0?cv_ct_cx=esp32&dchild=1&keywords=esp32&pd_rd_i=B079PVCF2G&pd_rd_r=a13a37f2-296c-44a8-9513-68ab5bfbd9ad&pd_rd_w=ehkKD&pd_rd_wg=YDw1j&pf_rd_p=52f9c563-bb87-44f4-9d9d-e1c03402d90f&pf_rd_r=R9PF5M9ACRSXKMZDT689&psc=1&qid=1611217599&sr=1-1-d3e58e83-6458-471c-a87e-175495b96a10)

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
    
## Wiring an Ethernet Port
W5500 Lite: <br>
ESP32 Pin 23 -> MOSI <br>
ESP32 Pin 19 -> MISO <br>
ESP32 Pin 18 -> SCLK <br>
ESP32 Pin 5  -> CS <br>
ESP32 Pin 26 -> RST <br>
