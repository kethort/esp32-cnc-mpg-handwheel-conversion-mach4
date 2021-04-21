#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h> 
#include <ModbusIP_ESP8266.h>
#include <ESP32Encoder.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

ModbusIP mb;
int regs[] = {1,  2,  3,  9,  10, 11, 4,  5,  6,  7, 12, 13, 14}; 

ESP32Encoder encoder;

//Pangodream_18650_CL BL;

#include "FS.h"
#include <SPI.h>
#include <TFT_eSPI.h> 
#include <TFT_eSPI_TouchUI.h>

TFT_eSPI tft = TFT_eSPI();       
TFT_eSPI_TouchUI slider[3];

#define DEBUG // enables Serial

#define SLIDER_MIN 0
#define SLIDER_MAX 100
#define SLIDER_BTN_W 20
#define SLIDER_BAR_L 180

// This is the file name used to store the touch coordinate
// calibration data. Change the name to start a new calibration.
#define CALIBRATION_FILE "/TouchCalData0"

// Set REPEAT_CAL to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
#define REPEAT_CAL false

#define AXISBUTTON_W 72
#define AXISBUTTON_H 50

#define NAVBUTTON_W 92
#define NAVBUTTON_H 50

#define XAXISBUTTON_X 5
#define XAXISBUTTON_Y 35

#define YAXISBUTTON_X 85
#define YAXISBUTTON_Y 35

#define ZAXISBUTTON_X 164
#define ZAXISBUTTON_Y 35

#define AAXISBUTTON_X 5
#define AAXISBUTTON_Y 95

#define BAXISBUTTON_X 85
#define BAXISBUTTON_Y 95

#define CAXISBUTTON_X 164
#define CAXISBUTTON_Y 95

#define INC1BUTTON_X 5
#define INC1BUTTON_Y 195

#define INC2BUTTON_X 85
#define INC2BUTTON_Y 195

#define INC3BUTTON_X 164
#define INC3BUTTON_Y 195

#define MAINBUTTON_X 85
#define MAINBUTTON_Y 265

#define IP_ADDR_X 120
#define IP_ADDR_Y 20

#define BATTERY_LEVEL_X 110
#define BATTERY_LEVEL_Y 265

#define DROBUTTON_X 72
#define DROBUTTON_Y 75

#define MPGBUTTON_X 72
#define MPGBUTTON_Y 135

#define SLIDERBUTTON_X 72
#define SLIDERBUTTON_Y 195

#define MPGEN 32
#define SCRLED 27

byte lastAxis = 50;
byte lastInc = 50;
byte lastSlider = 50;
byte pageNum;
float lastDRODecimal[6];

#define SCR_TMOUT 30000
uint32_t screenTime = 0; 
bool screenActive = true;
bool droPageLoad;

void setup(void)
{
  tft.init();

  // Set the rotation before we calibrate
  tft.setRotation(0);

  // call screen calibration
  touch_calibrate();

  // clear screen
  tft.fillScreen(TFT_BLACK);

  WiFiManager wifiManager;
  wifiManager.autoConnect("MPG_Handwheel");

#ifdef DEBUG
  Serial.begin(115200);
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
#endif
  
  setupOTA("MPG_Handwheel");
  mb.server();

  pinMode(MPGEN, INPUT_PULLUP);
  pinMode(SCRLED, OUTPUT);

  // button state coils
  for(int i = 0; i < sizeof(regs)/sizeof(regs[0]); i++) {
    mb.addCoil(regs[i]); 
  }

  encoder.attachFullQuad(4, 2);
  encoder.clearCount();
  mb.addHreg(55); // encoder counts modbus register

  // DRO registers (99 - 110)
  for(int i = 99; i < 111; i++)
    mb.addHreg(i);

  // Slider registers (56 - 58)
  mb.addHreg(56);
  mb.addHreg(57);
  mb.addHreg(58);
  
  drawMainPage();

  slider[0].initSliderH(&tft, SLIDER_MIN, SLIDER_MAX, 50, 80, SLIDER_BTN_W, SLIDER_BAR_L, TFT_WHITE, TFT_DARKGREY, TFT_BLACK, 1, 0);
  digitalWrite(SCRLED, HIGH);
  screenTime = millis();
}

void loop() {
  ArduinoOTA.handle();
  mb.task();
  mb.Coil(regs[6], !digitalRead(MPGEN));

  // forces encoder count rollover to zero instead of int16_t max/min
  if((int16_t)encoder.getCount() > 32760 ||(int16_t)encoder.getCount() < -32760){
    encoder.clearCount();
    mb.Hreg(55, 0);
  }

  // only get encoder counts when the MPG enable button is pressed
  if(!digitalRead(MPGEN)) {
    encoder.resumeCount();
    mb.Hreg(55, (int16_t)encoder.getCount());
    screenTime = millis();
    digitalWrite(SCRLED, HIGH);
  } else {
    encoder.pauseCount();
  }

  // turn off the screen if no touch for 30 seconds
  if (millis() - screenTime >= SCR_TMOUT) {
    screenActive = false;
    digitalWrite(SCRLED, LOW);
  }
  
  switch(pageNum) {
    case 1:
      getTouchMainPage();
      break;
    case 2:
      updateDROs();
      getTouchDROPage();
      break;
    case 3:
      getTouchMPGPage();
      break;
    case 4:
      getTouchSliderPage();
      break;
  }
}

void setupOTA(const char* nameprefix) {
  // Configure the hostname
  uint16_t maxlen = strlen(nameprefix) + 7;
  char *fullhostname = new char[maxlen];
  uint8_t mac[6];
  WiFi.macAddress(mac);
  snprintf(fullhostname, maxlen, "%s-%02x%02x%02x", nameprefix, mac[3], mac[4], mac[5]);
  ArduinoOTA.setHostname(fullhostname);
  delete[] fullhostname;

  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    Serial.println("Start updating " + type);
  });
  
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) 
      Serial.println("\nAuth Failed");
    else if (error == OTA_BEGIN_ERROR) 
      Serial.println("\nBegin Failed");
    else if (error == OTA_CONNECT_ERROR) 
      Serial.println("\nConnect Failed");
    else if (error == OTA_RECEIVE_ERROR) 
      Serial.println("\nReceive Failed");
    else if (error == OTA_END_ERROR) 
      Serial.println("\nEnd Failed");
  });

  ArduinoOTA.begin();
}

void touch_calibrate() {
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // check file system exists
  if (!SPIFFS.begin()) {
    Serial.println("Formatting file system");
    SPIFFS.format();
    SPIFFS.begin();
  }

  // check if calibration file exists and size is correct
  if (SPIFFS.exists(CALIBRATION_FILE)) {
    if (REPEAT_CAL)
    {
      // Delete if we want to re-calibrate
      SPIFFS.remove(CALIBRATION_FILE);
    }
    else
    {
      File f = SPIFFS.open(CALIBRATION_FILE, "r");
      if (f) {
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    }
  }

  if (calDataOK && !REPEAT_CAL) {
    // calibration data valid
    tft.setTouch(calData);
  } else {
    // data not valid so recalibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch corners as indicated");

    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CAL) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    // store data
    File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}

void getTouchMainPage() {
  uint16_t x, y;

  if (tft.getTouch(&x, &y)) {
    screenTime = millis();
    
    if (!screenActive) {
      screenActive = true;
      digitalWrite(SCRLED, HIGH);
    }
    
    if ((x > DROBUTTON_X) && (x < (DROBUTTON_X + NAVBUTTON_W))) {
      if ((y > DROBUTTON_Y) && (y <= (DROBUTTON_Y + NAVBUTTON_H))) {
        drawDROPage();
      }
    }

    if ((x > MPGBUTTON_X) && (x < (MPGBUTTON_X + NAVBUTTON_W))) {
      if ((y > MPGBUTTON_Y) && (y <= (MPGBUTTON_Y + NAVBUTTON_H))) {
        drawMPGPage();
      }
    }

    if ((x > SLIDERBUTTON_X) && (x < (SLIDERBUTTON_X + NAVBUTTON_W))) {
      if ((y > SLIDERBUTTON_Y) && (y <= (SLIDERBUTTON_Y + NAVBUTTON_H))) {
        drawSliderPage();
      }
    }
  }
}

void drawMainPage() {
  pageNum = 1;
  
  tft.fillScreen(TFT_BLACK);
  
  char strBfr[80];
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  sprintf(strBfr, "IP: %s", WiFi.localIP().toString().c_str());
  tft.drawString(strBfr, IP_ADDR_X, IP_ADDR_Y); 

  drawMainNavButtons();
}

void drawMainNavButtons() {
  tft.fillRect(DROBUTTON_X, DROBUTTON_Y, NAVBUTTON_W, NAVBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("DRO's", DROBUTTON_X + (NAVBUTTON_W / 2), DROBUTTON_Y + (NAVBUTTON_H / 2)); 
  
  tft.fillRect(MPGBUTTON_X, MPGBUTTON_Y, NAVBUTTON_W, NAVBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("MPG", MPGBUTTON_X + (NAVBUTTON_W / 2), MPGBUTTON_Y + (NAVBUTTON_H / 2)); 
  
  tft.fillRect(SLIDERBUTTON_X, SLIDERBUTTON_Y, NAVBUTTON_W, NAVBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Sliders", SLIDERBUTTON_X + (NAVBUTTON_W / 2), SLIDERBUTTON_Y + (NAVBUTTON_H / 2)); 
}

void drawMainPageButton() {
   tft.fillRect(MAINBUTTON_X, MAINBUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
   tft.setTextColor(TFT_BLACK);
   tft.setTextSize(2);
   tft.setTextDatum(MC_DATUM);
   tft.drawString("Main", MAINBUTTON_X + (AXISBUTTON_W / 2), MAINBUTTON_Y + (AXISBUTTON_H / 2));
}
