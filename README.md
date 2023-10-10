# Mach4 Wireless (Wifi) MPG Handwheel Using ESP32 and Modbus TCP library

<p align="center">
  <img src="/img/IMG_1541.jpeg" width="700"/>
</p>

<p align="center">
  <img src="/img/mpg-ins.PNG" width="700"/>
</p>

## Demonstration & Assembly
https://www.youtube.com/watch?v=sGhttK0reT0&t=2s <br>
https://www.youtube.com/watch?v=5fpQ3xS1Tu8&t=2s

## ESP32 Firmware and Network Setup
1. Download NodeMCU-PyFlasher.exe program from here: https://github.com/marcelstoer/nodemcu-pyflasher/releases/tag/v5.0.0
2. Connect a USB to FTDI or CP2102 to FTL programmer to the ESP32 PCB at the programmer port. 
3. Select the COM port of the programmer in the NodeMCU program.
4. Select the Mach4MPGModbus-ILI9341-Display.ino.bin file from the fw-build folder and press the Flash NodeMCU button to flash the firmware to the ESP32.
5. Reboot the ESP32.

<p align="center">
  <img src="/img/fw-upload.PNG" width="500"/>
</p>

6. The ESP32 device should show up in the list of network access points. 

<p align="center">
  <img src="/img/IMG_1540.jpeg" width="250"/>
</p>

7. Connect to this access point and navigate to 192.168.4.1 in a browser. 
8. Press the "Configure Wifi" button.

<p align="center">
  <img src="/img/captive_portal.png" width="300"/>
</p>

9. Find your network access point and connect to it by providing your network credentials.

<p align="center">
  <img src="/img/captive_portal2.PNG" width="250"/>
</p>

## Mach4 Software Setup
1. Find the local IP address of the ESP32 (espressif). 

<p align="center">
  <img src="/img/ip_address.PNG" width="300"/>
</p>

2. Launch the MPGInstaller.exe from the dist folder of the repo.
3. Select the Mach4 profile you want to update, enter the IP address of the ESP32 and press install. (if the ESP32 device is already connected to the network, and in the ARP table, the IP address should auto-populate)

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

## Installer Build from Source
If you want to make changes to MPGInstaller.py and rebuild exe file:

1. Install Python
2. In elevated command line window from root directory of repo: ```pip install -r requirements.txt```
4. In command line window from root directory of repo: ```python setup.py py2exe```

The updated .exe file will be created in the dist/ folder. 


## PCB Gerber File & 3d print files for Display included
Original 3d print: https://www.thingiverse.com/thing:4539827

<p align="center">
  <img src="/img/PCB.PNG" width="300"/>
</p>

<p align="center">
  <img src="/img/PCB-exploded.png" width="700"/>
</p>

<p align="center">
  <img src="/img/PCB-programming-port.png" width="700"/>
</p>
