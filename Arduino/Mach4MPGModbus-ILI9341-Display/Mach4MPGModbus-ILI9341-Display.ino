#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h> 
#include <ModbusIP_ESP8266.h>
#include <ESP32Encoder.h>

ModbusIP mb;
int regs[] = {1,  2,  3,  9,  10, 11, 4,  5,  6,  7}; 

ESP32Encoder encoder;

#include "FS.h"
#include <SPI.h>
#include <TFT_eSPI.h> 
#include <TFT_eSPI_TouchUI.h>

TFT_eSPI tft = TFT_eSPI();       
TFT_eSPI_TouchUI slider[3];

#define SLIDER_MIN 0
#define SLIDER_MAX 100
#define SLIDER_BTN_W 20
#define SLIDER_BAR_L 180

// This is the file name used to store the touch coordinate
// calibration data. Change the name to start a new calibration.
#define CALIBRATION_FILE "/TouchCalData4"

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

#define DROBUTTON_X 72
#define DROBUTTON_Y 75

#define MPGBUTTON_X 72
#define MPGBUTTON_Y 135

#define SLIDERBUTTON_X 72
#define SLIDERBUTTON_Y 195

#define MPGEN 25

//#define DEBUG

byte lastAxis = 50;
byte lastInc = 50;
byte pageNum;
float lastDRODecimal[6];

#define LOOP_PERIOD 10   // DRO's update every 10 ms
uint32_t updateTime = 0; // time for next update

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
  
  mb.server();

  pinMode(MPGEN, INPUT_PULLUP);
  
  for(int i = 0; i < sizeof(regs)/sizeof(regs[0]); i++) {
    mb.addCoil(regs[i]); 
  }

  encoder.attachFullQuad(2, 4);
  encoder.clearCount();
  mb.addHreg(55); // encoder counts modbus register

  for(int i = 99; i < 111; i++)
    mb.addHreg(i);

  drawMainPage();
  updateTime = millis(); // Next update time

  slider[0].initSliderH(&tft, SLIDER_MIN, SLIDER_MAX, 42, 50, SLIDER_BTN_W, SLIDER_BAR_L, TFT_WHITE, TFT_DARKGREY, TFT_BLACK, 1, 0);
}

void loop() {
  mb.task();
  mb.Coil(regs[6], !digitalRead(25));

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

  switch(pageNum) {
    case 1:
      getTouchMainPage();
      break;
    case 2:
      if (updateTime <= millis()) {
        updateTime = millis() + LOOP_PERIOD;
        updateDROs();
      }
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

void getTouchMPGPage() {
  uint16_t x, y;

  if (tft.getTouch(&x, &y))
  { 
    if ((x > XAXISBUTTON_X) && (x < (XAXISBUTTON_X + AXISBUTTON_W))) {
      if ((y > XAXISBUTTON_Y) && (y <= (XAXISBUTTON_Y + AXISBUTTON_H))) {
        selectAxis(XAXISBUTTON_X, XAXISBUTTON_Y, "X");
        mb.Coil(regs[lastAxis], 0);
        lastAxis = 0;
        mb.Coil(regs[0], 1);
      }
    }

    if ((x > YAXISBUTTON_X) && (x < (YAXISBUTTON_X + AXISBUTTON_W))) {
      if ((y > YAXISBUTTON_Y) && (y <= (YAXISBUTTON_Y + AXISBUTTON_H))) {
        selectAxis(YAXISBUTTON_X, YAXISBUTTON_Y, "Y");
        mb.Coil(regs[lastAxis], 0);
        lastAxis = 1;
        mb.Coil(regs[1], 1);
      }
    }

    if ((x > ZAXISBUTTON_X) && (x < (ZAXISBUTTON_X + AXISBUTTON_W))) {
      if ((y > ZAXISBUTTON_Y) && (y <= (ZAXISBUTTON_Y + AXISBUTTON_H))) {
        selectAxis(ZAXISBUTTON_X, ZAXISBUTTON_Y, "Z");
        mb.Coil(regs[lastAxis], 0);
        lastAxis = 2;
        mb.Coil(regs[2], 1);
      }
    }

    if ((x > AAXISBUTTON_X) && (x < (AAXISBUTTON_X + AXISBUTTON_W))) {
      if ((y > AAXISBUTTON_Y) && (y <= (AAXISBUTTON_Y + AXISBUTTON_H))) {
        selectAxis(AAXISBUTTON_X, AAXISBUTTON_Y, "A");
        mb.Coil(regs[lastAxis], 0);
        lastAxis = 3;
        mb.Coil(regs[3], 1);
      }
    }

    if ((x > BAXISBUTTON_X) && (x < (BAXISBUTTON_X + AXISBUTTON_W))) {
      if ((y > BAXISBUTTON_Y) && (y <= (BAXISBUTTON_Y + AXISBUTTON_H))) {
        selectAxis(BAXISBUTTON_X, BAXISBUTTON_Y, "B");
        mb.Coil(regs[lastAxis], 0);
        lastAxis = 4;
        mb.Coil(regs[4], 1);
      }
    }

    if ((x > CAXISBUTTON_X) && (x < (CAXISBUTTON_X + AXISBUTTON_W))) {
      if ((y > CAXISBUTTON_Y) && (y <= (CAXISBUTTON_Y + AXISBUTTON_H))) {
        selectAxis(CAXISBUTTON_X, CAXISBUTTON_Y, "C");
        mb.Coil(regs[lastAxis], 0);
        lastAxis = 5;
        mb.Coil(regs[5], 1);
      }
    }

    if ((x > INC1BUTTON_X) && (x < (INC1BUTTON_X + AXISBUTTON_W))) {
      if ((y > INC1BUTTON_Y) && (y <= (INC1BUTTON_Y + AXISBUTTON_H))) {
        selectInc(INC1BUTTON_X, INC1BUTTON_Y, "0.001");
        mb.Coil(regs[lastInc], 0);
        lastInc = 7;
        mb.Coil(regs[7], 1);
      }
    }

    if ((x > INC2BUTTON_X) && (x < (INC2BUTTON_X + AXISBUTTON_W))) {
      if ((y > INC2BUTTON_Y) && (y <= (INC2BUTTON_Y + AXISBUTTON_H))) {
        selectInc(INC2BUTTON_X, INC2BUTTON_Y, "0.01");
        mb.Coil(regs[lastInc], 0);
        lastInc = 8;
        mb.Coil(regs[8], 1);
      }
    }

    if ((x > INC3BUTTON_X) && (x < (INC3BUTTON_X + AXISBUTTON_W))) {
      if ((y > INC3BUTTON_Y) && (y <= (INC3BUTTON_Y + AXISBUTTON_H))) {
        selectInc(INC3BUTTON_X, INC3BUTTON_Y, "0.1");
        mb.Coil(regs[lastInc], 0);
        lastInc = 9;
        mb.Coil(regs[9], 1);
      }
    }
  
    if ((x > MAINBUTTON_X) && (x < (MAINBUTTON_X + AXISBUTTON_W))) {
      if ((y > MAINBUTTON_Y) && (y <= (MAINBUTTON_Y + AXISBUTTON_H))) {
        drawMainPage();
      }
    }
  }
}

void getTouchMainPage() {
  uint16_t x, y;

  if (tft.getTouch(&x, &y)) {
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
  tft.drawString(strBfr, 120, 20); 

  drawMainNavButtons();
}

void drawDROValue(byte axisID, byte hreg1, byte hreg2, byte y_loc) {
  int16_t droPrefix = mb.Hreg(hreg1);
  int16_t droPostfix = mb.Hreg(hreg2);
  float droDecimal = droPrefix + (droPostfix / 10000.0);

  tft.setTextColor(TFT_WHITE);
  tft.drawFloat(droDecimal, 4, 140, y_loc);  
  lastDRODecimal[axisID] = droDecimal;  
}

void updateDROs() {
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(3);
  tft.setTextDatum(MC_DATUM);

  // erase the last value
  tft.drawFloat(lastDRODecimal[0], 4, 140, 30); 
  tft.drawFloat(lastDRODecimal[1], 4, 140, 70);
  tft.drawFloat(lastDRODecimal[2], 4, 140, 110);
  tft.drawFloat(lastDRODecimal[3], 4, 140, 150);
  tft.drawFloat(lastDRODecimal[4], 4, 140, 190);
  tft.drawFloat(lastDRODecimal[5], 4, 140, 230);

  tft.setTextColor(TFT_WHITE);
  tft.drawString("X: ", 40, 30);
  tft.drawString("Y: ", 40, 70);
  tft.drawString("Z: ", 40, 110);
  tft.drawString("A: ", 40, 150);
  tft.drawString("B: ", 40, 190);
  tft.drawString("C: ", 40, 230);
  
  drawDROValue(0, 99, 100, 30);
  drawDROValue(1, 101, 102, 70);
  drawDROValue(2, 103, 104, 110);
  drawDROValue(3, 105, 106, 150);
  drawDROValue(4, 107, 108, 190);
  drawDROValue(5, 109, 110, 230);
}

void drawDROPage() {
  pageNum = 2;
  tft.fillScreen(TFT_BLACK);
  drawMainPageButton();
}

void getTouchDROPage() {
  uint16_t x, y;

  if (tft.getTouch(&x, &y)) {
    if ((x > MAINBUTTON_X) && (x < (MAINBUTTON_X + AXISBUTTON_W))) {
        if ((y > MAINBUTTON_Y) && (y <= (MAINBUTTON_Y + AXISBUTTON_H))) {
          drawMainPage();
        }
      }
  }
}

void drawMPGPage() {
  pageNum = 3;

  tft.fillScreen(TFT_BLACK);
  drawAxisButtons();
  drawIncButtons();
  drawMainPageButton();

  switch(lastAxis) {
    case 0:
      selectAxis(XAXISBUTTON_X, XAXISBUTTON_Y, "X");
      break;
    case 1:
      selectAxis(YAXISBUTTON_X, YAXISBUTTON_Y, "Y");
      break;
    case 2:
      selectAxis(ZAXISBUTTON_X, ZAXISBUTTON_Y, "Z");
      break;
    case 3:
      selectAxis(AAXISBUTTON_X, AAXISBUTTON_Y, "A");
      break;
    case 4:
      selectAxis(BAXISBUTTON_X, BAXISBUTTON_Y, "B");
      break;
    case 5:
      selectAxis(CAXISBUTTON_X, CAXISBUTTON_Y, "C");
      break;
  }

  switch(lastInc) {
    case 7:
      selectInc(INC1BUTTON_X, INC1BUTTON_Y, "0.001");
      break;
    case 8:
      selectInc(INC2BUTTON_X, INC2BUTTON_Y, "0.01");
      break;
    case 9:
      selectInc(INC3BUTTON_X, INC3BUTTON_Y, "0.1");
      break;
  }
}

void drawSliderPage() {
  pageNum = 4;

  tft.fillScreen(TFT_BLACK);

  slider[0].drawSliderH(0);
  
  drawMainPageButton();
}

void getTouchSliderPage() {
  uint16_t x, y;

  if (tft.getTouch(&x, &y)) {
    if (slider[0].containsH(x, y)) {
      int16_t value = slider[0].getValueH(x); 
      slider[0].drawSliderH(value);
    }
    
    if ((x > MAINBUTTON_X) && (x < (MAINBUTTON_X + AXISBUTTON_W))) {
      if ((y > MAINBUTTON_Y) && (y <= (MAINBUTTON_Y + AXISBUTTON_H))) {
        slider[0].drawSliderH(100);
        drawMainPage();
      }
    }
  }
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

void drawIncButtons() {
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("MPG Increment:", 125, 170);
  
  tft.fillRect(INC1BUTTON_X, INC1BUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("0.001", INC1BUTTON_X + (AXISBUTTON_W / 2), INC1BUTTON_Y + (AXISBUTTON_H / 2));
  
  tft.fillRect(INC2BUTTON_X, INC2BUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("0.01", INC2BUTTON_X + (AXISBUTTON_W / 2), INC2BUTTON_Y + (AXISBUTTON_H / 2));
  
  tft.fillRect(INC3BUTTON_X, INC3BUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("0.1", INC3BUTTON_X + (AXISBUTTON_W / 2), INC3BUTTON_Y + (AXISBUTTON_H / 2));
}

void drawAxisButtons() {
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("MPG Axis:", 120, 20);
  
  tft.fillRect(XAXISBUTTON_X, XAXISBUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("X", XAXISBUTTON_X + (AXISBUTTON_W / 2), XAXISBUTTON_Y + (AXISBUTTON_H / 2)); 
  
  tft.fillRect(YAXISBUTTON_X, YAXISBUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Y", YAXISBUTTON_X + (AXISBUTTON_W / 2), YAXISBUTTON_Y + (AXISBUTTON_H / 2)); 
  
  tft.fillRect(ZAXISBUTTON_X, ZAXISBUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Z", ZAXISBUTTON_X + (AXISBUTTON_W / 2), ZAXISBUTTON_Y + (AXISBUTTON_H / 2)); 
  
  tft.fillRect(AAXISBUTTON_X, AAXISBUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("A", AAXISBUTTON_X + (AXISBUTTON_W / 2), AAXISBUTTON_Y + (AXISBUTTON_H / 2)); 
  
  tft.fillRect(BAXISBUTTON_X, BAXISBUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("B", BAXISBUTTON_X + (AXISBUTTON_W / 2), BAXISBUTTON_Y + (AXISBUTTON_H / 2)); 
  
  tft.fillRect(CAXISBUTTON_X, CAXISBUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("C", CAXISBUTTON_X + (AXISBUTTON_W / 2), CAXISBUTTON_Y + (AXISBUTTON_H / 2));
}

void selectAxis(int32_t x, int32_t y, const char *axisID) {
  drawAxisButtons();
  tft.fillRect(x, y, AXISBUTTON_W, AXISBUTTON_H, TFT_GREEN);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(axisID, x + (AXISBUTTON_W / 2), y + (AXISBUTTON_H / 2));
}

void selectInc(int32_t x, int32_t y, const char *axisID) {
  drawIncButtons();
  tft.fillRect(x, y, AXISBUTTON_W, AXISBUTTON_H, TFT_GREEN);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(axisID, x + (AXISBUTTON_W / 2), y + (AXISBUTTON_H / 2));
}
