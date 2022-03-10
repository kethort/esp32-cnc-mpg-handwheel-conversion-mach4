bool droPageLoad;

void getTouchDROPage() {
  uint16_t x, y;

  if (tft.getTouch(&x, &y)) {
    screenTime = millis();
    
    if (!screenActive) {
      screenActive = true;
      digitalWrite(SCRLED, HIGH);
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
  drawButtonOnScreen("Ctrl", CONTROLBUTTON_X, CONTROLBUTTON_Y, MIDBUTTON_W, MIDBUTTON_H);
  drawButtonOnScreen("MPG", MPGBUTTON_X, MPGBUTTON_Y, MIDBUTTON_W, MIDBUTTON_H);
  drawButtonOnScreen("Speed", SPEEDBUTTON_X, SPEEDBUTTON_Y, MIDBUTTON_W, MIDBUTTON_H);
}

void drawDROValue(byte axisID, byte hreg1, byte hreg2) {
  int16_t droPrefix = mb.Hreg(hreg1);
  int16_t droPostfix = mb.Hreg(hreg2);
  float droDecimal = droPrefix + (droPostfix / 10000.0);

  if(droDecimal != lastDRODecimal[axisID] || droPageLoad) {
    screenTime = millis();
    // erase the last value
    tft.setTextColor(TFT_BLACK);
    tft.drawFloat(lastDRODecimal[axisID], 4, 140, (30 * axisID) + 70); 
    // draw the current value  
    tft.setTextColor(TFT_WHITE);
    tft.drawFloat(droDecimal, 4, 140, (30 * axisID) + 70);  
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

  tft.setTextColor(TFT_WHITE);
  tft.drawString("X: ", 40, 70);
  tft.drawString("Y: ", 40, 100);
  tft.drawString("Z: ", 40, 130);
  tft.drawString("A: ", 40, 160);
  tft.drawString("B: ", 40, 190);
  tft.drawString("C: ", 40, 220);

  updateDROs();
  droPageLoad = false;
}
