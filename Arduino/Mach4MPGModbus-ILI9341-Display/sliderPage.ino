#define SPEEDDNBTN_X 5
#define SPEEDDNBTN_Y 125

#define SPEEDUPBTN_X 200
#define SPEEDUPBTN_Y 125

#define SPINDLCCW_X 56
#define SPINDLCCW_Y 125

#define SPINDLCW_X 123
#define SPINDLCW_Y 125

#define FROBUTTON_X 5
#define FROBUTTON_Y 195

#define RROBUTTON_X 85
#define RROBUTTON_Y 195

#define SROBUTTON_X 164
#define SROBUTTON_Y 195

byte lastSlider = 0;

void drawSliderPage() {
  pageNum = 4;
  
  tft.fillScreen(TFT_BLACK);

  drawSliderButtons();
  drawButtonOnScreen("Back", BACKBUTTON_X, BACKBUTTON_Y, MIDBUTTON_W, NAVBUTTON_H);
}

void getTouchSliderPage() {
  uint16_t x, y;

  // using the MPG control reg and Mach4 status regs to 
  // delegate timing in communication 
  if (mb.Hreg(114) == 555) {
      mb.Hreg(59, -1);
      return;
  }   
  
  if (tft.getTouch(&x, &y)) {
    screenTime = millis();
    
    if (!screenActive) {
      screenActive = true;
      digitalWrite(SCRLED, HIGH);
    }
    
    if (slider[0].containsH(x, y)) {
      int16_t value = slider[0].getValueH(x); 
      slider[0].drawSliderH(value);
      updateSlider(value);
    }

    if (boundingBoxPressed(x, y, SPEEDDNBTN_X, (SPEEDDNBTN_X + SMALLBTN_W), SPEEDDNBTN_Y, (SPEEDDNBTN_Y + SMALLBTN_H))) {
      incrementSlider(-1);
    }

    if (boundingBoxPressed(x, y, SPEEDUPBTN_X, (SPEEDUPBTN_X + SMALLBTN_W), SPEEDUPBTN_Y, (SPEEDUPBTN_Y + SMALLBTN_H))) {
      incrementSlider(1);
    }

    if (boundingBoxPressed(x, y, FROBUTTON_X, (FROBUTTON_X + MIDBUTTON_W), FROBUTTON_Y, (FROBUTTON_Y + MIDBUTTON_H))) {
      tft.fillRect(SPINDLCCW_X, SPINDLCCW_Y, (SPINDLCW_X  + MIDBUTTON_W), MIDBUTTON_H, TFT_BLACK);
      selectRate(FROBUTTON_X, FROBUTTON_Y, "FRO%", 56, 10);
    }

    if (boundingBoxPressed(x, y, RROBUTTON_X, (RROBUTTON_X + MIDBUTTON_W), RROBUTTON_Y, (RROBUTTON_Y + MIDBUTTON_H))) {
      tft.fillRect(SPINDLCCW_X, SPINDLCCW_Y, (SPINDLCW_X  + MIDBUTTON_W), MIDBUTTON_H, TFT_BLACK);
      selectRate(RROBUTTON_X, RROBUTTON_Y, "RRO%", 57, 11);
    }

    if (boundingBoxPressed(x, y, SROBUTTON_X, (SROBUTTON_X + MIDBUTTON_W), SROBUTTON_Y, (SROBUTTON_Y + MIDBUTTON_H))) {
      drawButtonOnScreen("CCW", SPINDLCCW_X, SPINDLCCW_Y, (MIDBUTTON_W - 10), MIDBUTTON_H);
      drawButtonOnScreen("CW", SPINDLCW_X, SPINDLCW_Y, (MIDBUTTON_W - 10), MIDBUTTON_H);
      selectRate(SROBUTTON_X, SROBUTTON_Y, "SRO%", 58, 12);
    }

    if (boundingBoxPressed(x, y, BACKBUTTON_X, (BACKBUTTON_X + MIDBUTTON_W), BACKBUTTON_Y, (BACKBUTTON_Y + MIDBUTTON_H))) {
      slider[0].drawSliderH(100);
      lastSlider = 0;
      mb.Coil(regs[10], 0);
      mb.Coil(regs[11], 0);
      mb.Coil(regs[12], 0);
      drawDROPage();
    }
  }
}

void selectRate(int32_t x, int32_t y, const char *sliderName, uint8_t reg, uint8_t sliderNum) {
  highlightButton(&drawSliderButtons, x, y, MIDBUTTON_W, MIDBUTTON_H, sliderName);

  int16_t regVal = mb.Hreg(reg);
  drawSliderChange(regVal);
  selectSlider(sliderNum);
}

void updateSlider(int16_t sliderVal) {
  int16_t regVal;

  switch(lastSlider) {
    case 10:
      mb.Hreg(56, sliderVal);
      break;
    case 11:
      mb.Hreg(57, sliderVal);
      break;
    case 12:
      mb.Hreg(58, sliderVal);
      break;
  }
}

void incrementSlider(int8_t inc) {
  uint8_t sliderVal = 0;
  
  switch (lastSlider) {
    case 10:
      sliderVal = ((mb.Hreg(56) == 0) && (inc < 0)) ? 0 : mb.Hreg(56) + inc;
      break;
    case 11:
      sliderVal = ((mb.Hreg(57) == 0) && (inc < 0)) ? 0 : mb.Hreg(57) + inc;
      break;
    case 12:
      sliderVal = ((mb.Hreg(58) == 0) && (inc < 0)) ? 0 : mb.Hreg(58) + inc;
      break;
  }
  sliderVal = constrain(sliderVal, 0, 100);

  updateSlider(sliderVal);
  slider[0].drawSliderH(sliderVal);
}

void selectSlider(byte newSliderID) {
  mb.Coil(regs[lastSlider], 0);
  lastSlider = newSliderID;
  mb.Coil(regs[newSliderID], 1);  
}

void drawSliderChange(int16_t regVal) {
  slider[0].drawSliderH(regVal);
}

void drawSliderButtons() {
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("FeedRate Sliders:", 120, 20);

  drawButtonOnScreen("-", SPEEDDNBTN_X, SPEEDDNBTN_Y, SMALLBTN_W, SMALLBTN_H);
  drawButtonOnScreen("+", SPEEDUPBTN_X, SPEEDUPBTN_Y, SMALLBTN_W, SMALLBTN_H);    
  drawButtonOnScreen("FRO%", FROBUTTON_X, FROBUTTON_Y, MIDBUTTON_W, MIDBUTTON_H);
  drawButtonOnScreen("RRO%", RROBUTTON_X, RROBUTTON_Y, MIDBUTTON_W, MIDBUTTON_H);
  drawButtonOnScreen("SRO%", SROBUTTON_X, SROBUTTON_Y, MIDBUTTON_W, MIDBUTTON_H);
}
