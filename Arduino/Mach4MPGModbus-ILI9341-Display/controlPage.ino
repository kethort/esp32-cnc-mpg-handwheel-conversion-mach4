void drawControlPage() {
  pageNum = 2;

  tft.fillScreen(TFT_BLACK);
  
  drawBackButton();
}

void getTouchControlPage() {
  uint16_t x, y;

  if (tft.getTouch(&x, &y)) {
    screenTime = millis();
    
    if (!screenActive) {
      screenActive = true;
      digitalWrite(SCRLED, HIGH);
    }

    if ((x > BACKBUTTON_X) && (x < (BACKBUTTON_X + AXISBUTTON_W))) {
      if ((y > BACKBUTTON_Y) && (y <= (BACKBUTTON_Y + AXISBUTTON_H))) {
        drawDROPage();
      }
    }   
  }
}
