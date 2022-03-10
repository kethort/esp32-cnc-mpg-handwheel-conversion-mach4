#define ENABLEBUTTON_X 5
#define ENABLEBUTTON_Y 5

#define HOMEBUTTON_X 85
#define HOMEBUTTON_Y 5

#define STARTBUTTON_X 164
#define STARTBUTTON_Y 5

#define PAUSEBUTTON_X 5
#define PAUSEBUTTON_Y 65

#define AXESBUTTON_X 85
#define AXESBUTTON_Y 65

#define BATTBUTTON_X 164
#define BATTBUTTON_Y 65

void drawControlPage() {
  pageNum = 2;

  tft.fillScreen(TFT_BLACK);

  drawControlButtons();
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

void drawControlButtons() {
  drawButtonOnScreen("ENABLE", ENABLEBUTTON_X, ENABLEBUTTON_Y, MIDBUTTON_W, MIDBUTTON_H);
  drawButtonOnScreen("HOME", HOMEBUTTON_X, HOMEBUTTON_Y, MIDBUTTON_W, MIDBUTTON_H);
  drawButtonOnScreen("START", STARTBUTTON_X, STARTBUTTON_Y, MIDBUTTON_W, MIDBUTTON_H);
  drawButtonOnScreen("PAUSE", PAUSEBUTTON_X, PAUSEBUTTON_Y, MIDBUTTON_W, MIDBUTTON_H);
  drawButtonOnScreen("AXES", AXESBUTTON_X, AXESBUTTON_Y, MIDBUTTON_W, MIDBUTTON_H);
  drawButtonOnScreen("BATT", BATTBUTTON_X, BATTBUTTON_Y, MIDBUTTON_W, MIDBUTTON_H);
}
