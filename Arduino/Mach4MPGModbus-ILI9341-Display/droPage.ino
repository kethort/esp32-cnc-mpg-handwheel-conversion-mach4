#define XZEROBUTTON_X 5
#define XZEROBUTTON_Y 35

#define YZEROBUTTON_X 5
#define YZEROBUTTON_Y 75

#define ZZEROBUTTON_X 5
#define ZZEROBUTTON_Y 115

#define AZEROBUTTON_X 5
#define AZEROBUTTON_Y 155

#define BZEROBUTTON_X 5
#define BZEROBUTTON_Y 195

#define CZEROBUTTON_X 5
#define CZEROBUTTON_Y 235

#define ZEROBUTTON_W 72
#define ZEROBUTTON_H 32

bool droPageLoad;

void getTouchDROPage() {
  uint16_t x, y;

  if (tft.getTouch(&x, &y)) {
    screenTime = millis();
    
    if (!screenActive) {
      screenActive = true;
      digitalWrite(SCRLED, HIGH);
    }

    if (boundingBoxPressed(x, y, XZEROBUTTON_X, (XZEROBUTTON_X + ZEROBUTTON_W), XZEROBUTTON_Y, (XZEROBUTTON_Y + ZEROBUTTON_H))) {
      zeroAxis(XZEROBUTTON_X, XZEROBUTTON_Y, "X:");
    }

    if (boundingBoxPressed(x, y, YZEROBUTTON_X, (YZEROBUTTON_X + ZEROBUTTON_W), YZEROBUTTON_Y, (YZEROBUTTON_Y + ZEROBUTTON_H))) {
      zeroAxis(YZEROBUTTON_X, YZEROBUTTON_Y, "Y:");
    }

    if (boundingBoxPressed(x, y, ZZEROBUTTON_X, (ZZEROBUTTON_X + ZEROBUTTON_W), ZZEROBUTTON_Y, (ZZEROBUTTON_Y + ZEROBUTTON_H))) {
      zeroAxis(ZZEROBUTTON_X, ZZEROBUTTON_Y, "Z:");
    }

    if (boundingBoxPressed(x, y, AZEROBUTTON_X, (AZEROBUTTON_X + ZEROBUTTON_W), AZEROBUTTON_Y, (AZEROBUTTON_Y + ZEROBUTTON_H))) {
      zeroAxis(AZEROBUTTON_X, AZEROBUTTON_Y, "A:");
    }

    if (boundingBoxPressed(x, y, BZEROBUTTON_X, (BZEROBUTTON_X + ZEROBUTTON_W), BZEROBUTTON_Y, (BZEROBUTTON_Y + ZEROBUTTON_H))) {
      zeroAxis(BZEROBUTTON_X, BZEROBUTTON_Y, "B:");
    }

    if (boundingBoxPressed(x, y, CZEROBUTTON_X, (CZEROBUTTON_X + ZEROBUTTON_W), CZEROBUTTON_Y, (CZEROBUTTON_Y + ZEROBUTTON_H))) {
      zeroAxis(CZEROBUTTON_X, CZEROBUTTON_Y, "C:");
    }
    
    if (boundingBoxPressed(x, y, CONTROLBUTTON_X, (CONTROLBUTTON_X + MIDBUTTON_W), CONTROLBUTTON_Y, (CONTROLBUTTON_Y + MIDBUTTON_H))) {
      drawControlPage();
    }

    if (boundingBoxPressed(x, y, MPGBUTTON_X, (MPGBUTTON_X + MIDBUTTON_W), MPGBUTTON_Y, (MPGBUTTON_Y + MIDBUTTON_H))) {
      drawMPGPage();
    }

    if (boundingBoxPressed(x, y, SPEEDBUTTON_X, (SPEEDBUTTON_X + MIDBUTTON_W), SPEEDBUTTON_Y, (SPEEDBUTTON_Y + MIDBUTTON_H))) {
      drawSliderPage();
    }
  }
}

void drawDROPage() {
  pageNum = 1;
  
  tft.fillScreen(TFT_BLACK);
  
  char strBfr[50];
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  sprintf(strBfr, "%s", WiFi.localIP().toString().c_str());
  tft.drawString(strBfr, IP_ADDR_X, IP_ADDR_Y); 

  drawDROs();
  drawDRONavButtons();
}

void drawDRONavButtons() {
  drawButtonOnScreen("Ctrl", CONTROLBUTTON_X, CONTROLBUTTON_Y, MIDBUTTON_W, NAVBUTTON_H);
  drawButtonOnScreen("MPG", MPGBUTTON_X, MPGBUTTON_Y, MIDBUTTON_W, NAVBUTTON_H);
  drawButtonOnScreen("Speed", SPEEDBUTTON_X, SPEEDBUTTON_Y, MIDBUTTON_W, NAVBUTTON_H);
}

void drawDROValue(byte axisID, byte hreg1, byte hreg2) {
  int16_t droPrefix = mb.Hreg(hreg1);
  int16_t droPostfix = mb.Hreg(hreg2);
  float droDecimal = droPrefix + (droPostfix / 10000.0);

  if(droDecimal != lastDRODecimal[axisID] || droPageLoad) {
    screenTime = millis();
    // erase the last value
    tft.setTextColor(TFT_BLACK);
    tft.drawFloat(lastDRODecimal[axisID], 4, 145, (40 * axisID) + 54); 
    // draw the current value  
    tft.setTextColor(TFT_WHITE);
    tft.drawFloat(droDecimal, 4, 145, (40 * axisID) + 54);  
  }
  lastDRODecimal[axisID] = droDecimal;  
}

void updateDROs() { 
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  
  drawDROValue(0, 99, 100);
  drawDROValue(1, 101, 102);
  drawDROValue(2, 103, 104);
  drawDROValue(3, 105, 106);
  drawDROValue(4, 107, 108);
  drawDROValue(5, 109, 110);
}

void drawDROs() {
  droPageLoad = true;
  
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);

  drawZeroButtons();
  updateDROs();
  droPageLoad = false;
}

void drawZeroButtons() {
  drawButtonOnScreen("X:", XZEROBUTTON_X, XZEROBUTTON_Y, ZEROBUTTON_W, ZEROBUTTON_H);
  drawButtonOnScreen("Y:", YZEROBUTTON_X, YZEROBUTTON_Y, ZEROBUTTON_W, ZEROBUTTON_H);
  drawButtonOnScreen("Z:", ZZEROBUTTON_X, ZZEROBUTTON_Y, ZEROBUTTON_W, ZEROBUTTON_H);
  drawButtonOnScreen("A:", AZEROBUTTON_X, AZEROBUTTON_Y, ZEROBUTTON_W, ZEROBUTTON_H);
  drawButtonOnScreen("B:", BZEROBUTTON_X, BZEROBUTTON_Y, ZEROBUTTON_W, ZEROBUTTON_H);
  drawButtonOnScreen("C:", CZEROBUTTON_X, CZEROBUTTON_Y, ZEROBUTTON_W, ZEROBUTTON_H);
}

void zeroAxis(int32_t x, int32_t y, const char *axisID) {
  drawZeroButtons();
  tft.fillRect(x, y, ZEROBUTTON_W, ZEROBUTTON_H, TFT_GREEN);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(axisID, x + (ZEROBUTTON_W / 2), y + (ZEROBUTTON_H / 2));
}
