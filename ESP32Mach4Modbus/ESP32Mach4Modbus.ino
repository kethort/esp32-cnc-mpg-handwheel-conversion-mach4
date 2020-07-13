#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h> 
#include <ModbusIP_ESP8266.h>
#include <ESP32Encoder.h>

ModbusIP mb;
int regs[] = {1, 2, 3, 4, 5, 6, 7}; 
int pins[] = {13, 14, 27, 25, 26, 23, 19};

ESP32Encoder encoder;

void setup() {
  Serial.begin(115200);
 
  WiFiManager wifiManager;

  IPAddress _ip = IPAddress(192, 168, 1, 222);
  IPAddress _gw = IPAddress(192, 168, 1, 1);
  IPAddress _sn = IPAddress(255, 255, 255, 0);
  
  wifiManager.setSTAStaticIPConfig(_ip, _gw, _sn);
  
  wifiManager.autoConnect("AutoConnectAP");

  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
        
  mb.server();
  
  for(int i = 0; i < sizeof(regs)/sizeof(regs[0]); i++) {
    pinMode(pins[i], INPUT_PULLUP);
    mb.addCoil(regs[i]); 
  } 

  encoder.attachFullQuad(2, 4);
  encoder.clearCount();
  mb.addHreg(55);
}
 
void loop() {
  mb.task();
   
  for(int i = 0; i < sizeof(regs)/sizeof(regs[0]); i++)
    mb.Coil(regs[i], !digitalRead(pins[i]));

  if(!digitalRead(25))
    mb.Hreg(55, (int16_t)encoder.getCount());
  else {
    encoder.clearCount();
    mb.Hreg(55, 0); 
  }
}
