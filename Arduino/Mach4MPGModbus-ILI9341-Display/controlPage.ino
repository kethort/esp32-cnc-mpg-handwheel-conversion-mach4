void drawControlPage() {
  pageNum = 2;

  tft.fillScreen(TFT_BLACK);
  
  drawButtonOnScreen("Back", BACKBUTTON_X, BACKBUTTON_Y, MIDBUTTON_W, MIDBUTTON_H);
}

void getTouchControlPage() {
  uint16_t x, y;

  if (tft.getTouch(&x, &y)) {
    screenTime = millis();
    
    if (!screenActive) {
      screenActive = true;
      digitalWrite(SCRLED, HIGH);
    }

    if (boundingBoxPressed(x, y, BACKBUTTON_X, (BACKBUTTON_X + MIDBUTTON_W), BACKBUTTON_Y, (BACKBUTTON_Y + MIDBUTTON_H))) {
      drawDROPage();
    }  
  }
}
