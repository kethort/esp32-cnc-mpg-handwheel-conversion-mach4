void drawControlPage() {
  pageNum = 2;

  tft.fillScreen(TFT_BLACK);
  
  drawMainPageButton();
}

void getTouchControlPage() {
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
