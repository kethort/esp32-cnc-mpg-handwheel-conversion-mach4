#define ZEROAXISBUTTON_X 75
#define ZEROAXISBUTTON_Y 65

#define GOTO0AXISBUTTON_X 75
#define GOTO0AXISBUTTON_Y 125

#define AXISBUTTON_W 92

void drawAxisControlPage(const char *axis) {
  pageNum = 5;

  tft.fillScreen(TFT_BLACK);

  drawAxisLabel(axis);
  drawAxisControlButtons();
  drawButtonOnScreen("Back", BACKBUTTON_X, BACKBUTTON_Y, MIDBUTTON_W, NAVBUTTON_H);
}

void getTouchAxisControlPage() {
  uint16_t x, y;

  if (tft.getTouch(&x, &y)) {
    screenTime = millis();
    
    if (!screenActive) {
      screenActive = true;
      digitalWrite(SCRLED, HIGH);
    }

    if (boundingBoxPressed(x, y, ZEROAXISBUTTON_X, (ZEROAXISBUTTON_X + AXISBUTTON_W), ZEROAXISBUTTON_Y, (ZEROAXISBUTTON_Y + MIDBUTTON_H))) {
      // update cntrl reg to zero axis 
      // mach must not be in cycle
      drawDROPage();
    } 
    
    if (boundingBoxPressed(x, y, GOTO0AXISBUTTON_X, (GOTO0AXISBUTTON_X + AXISBUTTON_W), GOTO0AXISBUTTON_Y, (GOTO0AXISBUTTON_Y + MIDBUTTON_H))) {
      // update cntrl reg to goto zero 
      // mach must be enabled and not in cycle
      drawDROPage();
    } 
    
    if (boundingBoxPressed(x, y, BACKBUTTON_X, (BACKBUTTON_X + MIDBUTTON_W), BACKBUTTON_Y, (BACKBUTTON_Y + MIDBUTTON_H))) {
      drawDROPage();
    }  
  }
}

void drawAxisLabel(const char *axis) {
  const char *label = " Axis: ";
  uint16_t labelLen = strlen(axis) + strlen(label);
  char *fullLabel = new char[labelLen];
  snprintf(fullLabel, labelLen, "%s%s", axis, label);  

  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(fullLabel, 120, 20);
}

void drawAxisControlButtons() {
  drawButtonOnScreen("ZERO", ZEROAXISBUTTON_X, ZEROAXISBUTTON_Y, AXISBUTTON_W, MIDBUTTON_H);
  drawButtonOnScreen("GOTO 0", GOTO0AXISBUTTON_X, GOTO0AXISBUTTON_Y, AXISBUTTON_W, MIDBUTTON_H);
}
