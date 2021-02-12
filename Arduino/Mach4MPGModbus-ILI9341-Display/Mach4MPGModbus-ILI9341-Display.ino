#include <WiFi.h>
#include <DNSServer.h>
#include <WebServer.h>
#include <WiFiManager.h> 
#include <ModbusIP_ESP8266.h>
#include <ESP32Encoder.h>

ModbusIP mb;
int regs[] = {1,  2,  3,  9,  10, 11, 4,  5,  6,  7}; 

byte currentPin;

ESP32Encoder encoder;

#include "FS.h"
#include <SPI.h>
#include <TFT_eSPI.h> 

TFT_eSPI tft = TFT_eSPI();       

// This is the file name used to store the touch coordinate
// calibration data. Cahnge the name to start a new calibration.
#define CALIBRATION_FILE "/TouchCalData4"

// Set REPEAT_CAL to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
#define REPEAT_CAL false

#define AXISBUTTON_W 72
#define AXISBUTTON_H 50

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

#define PREVBUTTON_X 5
#define PREVBUTTON_Y 265

#define NEXTBUTTON_X 164
#define NEXTBUTTON_Y 265

byte lastAxis;
//byte currentAxis;
byte lastInc;
//byte currentInc;

void setup(void)
{
  Serial.begin(115200);
  tft.init();

  // Set the rotation before we calibrate
  tft.setRotation(0);

  // call screen calibration
  touch_calibrate();

  // clear screen
  tft.fillScreen(TFT_BLACK);

  drawAxisButtons();
  drawIncButtons();
  drawPrevNextButtons(); 

  WiFiManager wifiManager;
  wifiManager.autoConnect("MPG_Handwheel");

  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
  mb.server();

  pinMode(25, INPUT_PULLUP);
  
  for(int i = 0; i < sizeof(regs)/sizeof(regs[0]); i++) {
    mb.addCoil(regs[i]); 
  }

  encoder.attachFullQuad(2, 4);
  encoder.clearCount();
  mb.addHreg(55); // encoder counts modbus register
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
  
  getTouchPageTwo();
}

void touch_calibrate() {
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // check file system exists
  if (!SPIFFS.begin()) {
    Serial.println("Formating file system");
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

void getTouchPageTwo() {
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
  }
}

void drawPrevNextButtons() {
   tft.fillRect(PREVBUTTON_X, PREVBUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
   tft.setTextColor(TFT_BLACK);
   tft.setTextSize(2);
   tft.setTextDatum(MC_DATUM);
   tft.drawString("<", PREVBUTTON_X + (AXISBUTTON_W / 2), PREVBUTTON_Y + (AXISBUTTON_H / 2));

   tft.fillRect(NEXTBUTTON_X, NEXTBUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
   tft.setTextColor(TFT_BLACK);
   tft.setTextSize(2);
   tft.setTextDatum(MC_DATUM);
   tft.drawString(">", NEXTBUTTON_X + (AXISBUTTON_W / 2), NEXTBUTTON_Y + (AXISBUTTON_H / 2));
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
