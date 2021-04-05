void drawDROValue(byte axisID, byte hreg1, byte hreg2) {
  int16_t droPrefix = mb.Hreg(hreg1);
  int16_t droPostfix = mb.Hreg(hreg2);
  float droDecimal = droPrefix + (droPostfix / 10000.0);

  if(droDecimal != lastDRODecimal[axisID] || droPageLoad) {
    screenTime = millis();
    tft.setTextColor(TFT_BLACK);
    // erase the last value
    tft.drawFloat(lastDRODecimal[axisID], 4, 140, (40 * axisID) + 30); 
      
    tft.setTextColor(TFT_WHITE);
    tft.drawFloat(droDecimal, 4, 140, (40 * axisID) + 30);  
  }
  lastDRODecimal[axisID] = droDecimal;  
}

void updateDROs() { 
  tft.setTextSize(3);
  tft.setTextDatum(MC_DATUM);
  
  drawDROValue(0, 99, 100);
  drawDROValue(1, 101, 102);
  drawDROValue(2, 103, 104);
  drawDROValue(3, 105, 106);
  drawDROValue(4, 107, 108);
  drawDROValue(5, 109, 110);
}

void drawDROPage() {
  pageNum = 2;
  droPageLoad = true;
  
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(3);
  tft.setTextDatum(MC_DATUM);

  tft.setTextColor(TFT_WHITE);
  tft.drawString("X: ", 40, 30);
  tft.drawString("Y: ", 40, 70);
  tft.drawString("Z: ", 40, 110);
  tft.drawString("A: ", 40, 150);
  tft.drawString("B: ", 40, 190);
  tft.drawString("C: ", 40, 230);

  updateDROs();
  droPageLoad = false;
  drawMainPageButton();
}

void getTouchDROPage() {
  uint16_t x, y;

  if (tft.getTouch(&x, &y)) {
    screenTime = millis();
    
    if (!screenActive) {
      screenActive = true;
      digitalWrite(SCRLED, HIGH);
    }
    
    if ((x > MAINBUTTON_X) && (x < (MAINBUTTON_X + AXISBUTTON_W))) {
        if ((y > MAINBUTTON_Y) && (y <= (MAINBUTTON_Y + AXISBUTTON_H))) {
          drawMainPage();
        }
    }
  }
}
