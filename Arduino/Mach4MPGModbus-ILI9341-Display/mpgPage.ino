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

byte lastAxis = 50;
byte lastInc = 50;

void drawMPGPage() {
  pageNum = 3;

  tft.fillScreen(TFT_BLACK);
  drawAxisButtons();
  drawIncButtons();
  drawButtonOnScreen("Back", BACKBUTTON_X, BACKBUTTON_Y, MIDBUTTON_W, NAVBUTTON_H);

  switch(lastAxis) {
    case 0:
      selectAxis(XAXISBUTTON_X, XAXISBUTTON_Y, "X", 50);
      break;
    case 1:
      selectAxis(YAXISBUTTON_X, YAXISBUTTON_Y, "Y", 50);
      break;
    case 2:
      selectAxis(ZAXISBUTTON_X, ZAXISBUTTON_Y, "Z", 50);
      break;
    case 3:
      selectAxis(AAXISBUTTON_X, AAXISBUTTON_Y, "A", 50);
      break;
    case 4:
      selectAxis(BAXISBUTTON_X, BAXISBUTTON_Y, "B", 50);
      break;
    case 5:
      selectAxis(CAXISBUTTON_X, CAXISBUTTON_Y, "C", 50);
      break;
  }

  switch(lastInc) {
    case 7:
      selectInc(INC1BUTTON_X, INC1BUTTON_Y, "0.001", 50);
      break;
    case 8:
      selectInc(INC2BUTTON_X, INC2BUTTON_Y, "0.01", 50);
      break;
    case 9:
      selectInc(INC3BUTTON_X, INC3BUTTON_Y, "0.1", 50);
      break;
  }
}

void getTouchMPGPage() {
  uint16_t x, y;

  if (tft.getTouch(&x, &y)) {
    screenTime = millis();
    
    if (!screenActive) {
      screenActive = true;
      digitalWrite(SCRLED, HIGH);
    }

    if (boundingBoxPressed(x, y, XAXISBUTTON_X, (XAXISBUTTON_X + MIDBUTTON_W), XAXISBUTTON_Y, (XAXISBUTTON_Y + MIDBUTTON_H))) {
      selectAxis(XAXISBUTTON_X, XAXISBUTTON_Y, "X", 0);
    }

    if (boundingBoxPressed(x, y, YAXISBUTTON_X, (YAXISBUTTON_X + MIDBUTTON_W), YAXISBUTTON_Y, (YAXISBUTTON_Y + MIDBUTTON_H))) {
      selectAxis(YAXISBUTTON_X, YAXISBUTTON_Y, "Y", 1);
    }

    if (boundingBoxPressed(x, y, ZAXISBUTTON_X, (ZAXISBUTTON_X + MIDBUTTON_W), ZAXISBUTTON_Y, (ZAXISBUTTON_Y + MIDBUTTON_H))) {
      selectAxis(ZAXISBUTTON_X, ZAXISBUTTON_Y, "Z", 2);
    }

    if (boundingBoxPressed(x, y, AAXISBUTTON_X, (AAXISBUTTON_X + MIDBUTTON_W), AAXISBUTTON_Y, (AAXISBUTTON_Y + MIDBUTTON_H))) {
      selectAxis(AAXISBUTTON_X, AAXISBUTTON_Y, "A", 3);
    }

    if (boundingBoxPressed(x, y, BAXISBUTTON_X, (BAXISBUTTON_X + MIDBUTTON_W), BAXISBUTTON_Y, (BAXISBUTTON_Y + MIDBUTTON_H))) {
      selectAxis(BAXISBUTTON_X, BAXISBUTTON_Y, "B", 4);
    }

    if (boundingBoxPressed(x, y, CAXISBUTTON_X, (CAXISBUTTON_X + MIDBUTTON_W), CAXISBUTTON_Y, (CAXISBUTTON_Y + MIDBUTTON_H))) {
      selectAxis(CAXISBUTTON_X, CAXISBUTTON_Y, "C", 5);
    }

    if (boundingBoxPressed(x, y, INC1BUTTON_X, (INC1BUTTON_X + MIDBUTTON_W), INC1BUTTON_Y, (INC1BUTTON_Y + MIDBUTTON_H))) {
      selectInc(INC1BUTTON_X, INC1BUTTON_Y, "0.001", 7);
    }

    if (boundingBoxPressed(x, y, INC2BUTTON_X, (INC2BUTTON_X + MIDBUTTON_W), INC2BUTTON_Y, (INC2BUTTON_Y + MIDBUTTON_H))) {
      selectInc(INC2BUTTON_X, INC2BUTTON_Y, "0.01", 8);
    }

    if (boundingBoxPressed(x, y, INC3BUTTON_X, (INC3BUTTON_X + MIDBUTTON_W), INC3BUTTON_Y, (INC3BUTTON_Y + MIDBUTTON_H))) {
      selectInc(INC3BUTTON_X, INC3BUTTON_Y, "0.1", 9);
    }

    if (boundingBoxPressed(x, y, BACKBUTTON_X, (BACKBUTTON_X + MIDBUTTON_W), BACKBUTTON_Y, (BACKBUTTON_Y + MIDBUTTON_H))) {
      drawDROPage();
    }
  }
}

void drawIncButtons() {
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("MPG Increment:", 125, 170);

  drawButtonOnScreen("0.001", INC1BUTTON_X, INC1BUTTON_Y, MIDBUTTON_W, MIDBUTTON_H);
  drawButtonOnScreen("0.01", INC2BUTTON_X, INC2BUTTON_Y, MIDBUTTON_W, MIDBUTTON_H);
  drawButtonOnScreen("0.1", INC3BUTTON_X, INC3BUTTON_Y, MIDBUTTON_W, MIDBUTTON_H);
}

void drawAxisButtons() {
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("MPG Axis:", 120, 20);

  drawButtonOnScreen("X", XAXISBUTTON_X, XAXISBUTTON_Y, MIDBUTTON_W, MIDBUTTON_H);
  drawButtonOnScreen("Y", YAXISBUTTON_X, YAXISBUTTON_Y, MIDBUTTON_W, MIDBUTTON_H);
  drawButtonOnScreen("Z", ZAXISBUTTON_X, ZAXISBUTTON_Y, MIDBUTTON_W, MIDBUTTON_H);
  drawButtonOnScreen("A", AAXISBUTTON_X, AAXISBUTTON_Y, MIDBUTTON_W, MIDBUTTON_H);
  drawButtonOnScreen("B", BAXISBUTTON_X, BAXISBUTTON_Y, MIDBUTTON_W, MIDBUTTON_H);
  drawButtonOnScreen("C", CAXISBUTTON_X, CAXISBUTTON_Y, MIDBUTTON_W, MIDBUTTON_H);
}

void selectAxis(int32_t x, int32_t y, const char *axisID, int8_t newAxis) {
  highlightButton(&drawAxisButtons, x, y, MIDBUTTON_W, MIDBUTTON_H, axisID);

  if (newAxis < 49) {
    mb.Coil(regs[lastAxis], 0);
    lastAxis = newAxis;
    mb.Coil(regs[newAxis], 1);
  }
}

void selectInc(int32_t x, int32_t y, const char *incID, int8_t newInc) {
  highlightButton(&drawIncButtons, x, y, MIDBUTTON_W, MIDBUTTON_H, incID);

  if (newInc < 49) {
    mb.Coil(regs[lastInc], 0);
    lastInc = newInc;
    mb.Coil(regs[newInc], 1);
  }
}
