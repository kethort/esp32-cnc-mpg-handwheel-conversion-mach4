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
