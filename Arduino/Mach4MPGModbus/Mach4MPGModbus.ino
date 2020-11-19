#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h> 
#include <ModbusIP_ESP8266.h>
#include <ESP32Encoder.h>
//#include <Pangodream_18650_CL.h>

//#define STATIC_IP

ModbusIP mb;
int regs[] = {1,  2,  3,  9,  10, 11, 4,  5,  6,  7}; 
int pins[] = {13, 14, 27, 32, 35, 34, 25, 15, 21, 22};

byte currentPin;

ESP32Encoder encoder;

//Pangodream_18650_CL BL(36, 1.7, 50); // battery voltage ADC input pin 36

void setup() {
  Serial.begin(115200);
  
  WiFiManager wifiManager;

#ifdef STATIC_IP
  IPAddress _ip = IPAddress(192, 168, 1, 222);
  IPAddress _gw = IPAddress(192, 168, 1, 1);
  IPAddress _sn = IPAddress(255, 255, 255, 0);
  
  wifiManager.setSTAStaticIPConfig(_ip, _gw, _sn);
#endif
  
  wifiManager.autoConnect("MPG_Handwheel");

  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
        
  mb.server();

  for(int i = 0; i < sizeof(regs)/sizeof(regs[0]); i++) {
    pinMode(pins[i], INPUT_PULLUP);
    mb.addCoil(regs[i]); 
  } 

  // inputs 2 and 4 are A+ and B+ on the MPG Handwheel
  encoder.attachFullQuad(2, 4);
  encoder.clearCount();
  mb.addHreg(55); // encoder counts modbus register
  mb.addHreg(56); // battery level register
}

void loop() {
  //mb.Hreg(56, BL.getBatteryChargeLevel());
  
  mb.task();
  
  for(int i = 0; i < sizeof(regs)/sizeof(regs[0]); i++)
  {
    // clear the encoder count if the axis input changes
    if (!digitalRead(pins[i]) && i < 6 && currentPin != pins[i]) {
        currentPin = pins[i];
        encoder.clearCount();
        mb.Hreg(55, 0);
    }
    
    mb.Coil(regs[i], !digitalRead(pins[i]));
  }

  // forces encoder count rollover to zero instead of int16_t max/min
  if((int16_t)encoder.getCount() > 32760 ||(int16_t)encoder.getCount() < -32760){
    encoder.clearCount();
    mb.Hreg(55, 0);
  }

  // only get encoder counts when the MPG enable button is pressed
  if(!digitalRead(25)) {
    encoder.resumeCount();
    mb.Hreg(55, (int16_t)encoder.getCount());
  } else {
    encoder.pauseCount();
  }
}
