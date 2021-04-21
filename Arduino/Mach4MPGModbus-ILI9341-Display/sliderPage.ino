void selectSlider(int32_t x, int32_t y, const char *sliderName) {
  drawSliderButtons();
  tft.fillRect(x, y, AXISBUTTON_W, AXISBUTTON_H, TFT_GREEN);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(sliderName, x + (AXISBUTTON_W / 2), y + (AXISBUTTON_H / 2));
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

void drawSliderPage() {
  pageNum = 3;
  
  tft.fillScreen(TFT_BLACK);
  slider[0].drawSliderH(0);
  
  drawSliderButtons();
  drawMainPageButton();
}

void getTouchSliderPage() {
  uint16_t x, y;

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

    if ((x > INC1BUTTON_X) && (x < (INC1BUTTON_X + AXISBUTTON_W))) {
      if ((y > INC1BUTTON_Y) && (y <= (INC1BUTTON_Y + AXISBUTTON_H))) {
        selectSlider(INC1BUTTON_X, INC1BUTTON_Y, "FRO%"); 
        int16_t regVal = mb.Hreg(56);
        slider[0].drawSliderH(100);
        slider[0].drawSliderH(0);
        slider[0].drawSliderH(regVal);
        mb.Coil(regs[lastSlider], 0);
        lastSlider = 10;
        mb.Coil(regs[10], 1);
      }
    }

    if ((x > INC2BUTTON_X) && (x < (INC2BUTTON_X + AXISBUTTON_W))) {
      if ((y > INC2BUTTON_Y) && (y <= (INC2BUTTON_Y + AXISBUTTON_H))) {
        selectSlider(INC2BUTTON_X, INC2BUTTON_Y, "RRO%");
        int16_t regVal = mb.Hreg(57);
        slider[0].drawSliderH(100);
        slider[0].drawSliderH(0);
        slider[0].drawSliderH(regVal);
        mb.Coil(regs[lastSlider], 0);
        lastSlider = 11;
        mb.Coil(regs[11], 1);
      }
    }

    if ((x > INC3BUTTON_X) && (x < (INC3BUTTON_X + AXISBUTTON_W))) {
      if ((y > INC3BUTTON_Y) && (y <= (INC3BUTTON_Y + AXISBUTTON_H))) {
        selectSlider(INC3BUTTON_X, INC3BUTTON_Y, "SRO%");
        int16_t regVal = mb.Hreg(58);
        slider[0].drawSliderH(100);
        slider[0].drawSliderH(0);
        slider[0].drawSliderH(regVal);
        mb.Coil(regs[lastSlider], 0);
        lastSlider = 12;
        mb.Coil(regs[12], 1);
      }
    }
    
    if ((x > MAINBUTTON_X) && (x < (MAINBUTTON_X + AXISBUTTON_W))) {
      if ((y > MAINBUTTON_Y) && (y <= (MAINBUTTON_Y + AXISBUTTON_H))) {
        slider[0].drawSliderH(100);
        lastSlider = 0;
        mb.Coil(regs[10], 0);
        mb.Coil(regs[11], 0);
        mb.Coil(regs[12], 0);
        drawMainPage();
      }
    }
  }
}

void drawSliderButtons() {
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("FeedRate Sliders:", 120, 20);
   
  tft.fillRect(INC1BUTTON_X, INC1BUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("FRO%", INC1BUTTON_X + (AXISBUTTON_W / 2), INC1BUTTON_Y + (AXISBUTTON_H / 2));
  
  tft.fillRect(INC2BUTTON_X, INC2BUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("RRO%", INC2BUTTON_X + (AXISBUTTON_W / 2), INC2BUTTON_Y + (AXISBUTTON_H / 2));
  
  tft.fillRect(INC3BUTTON_X, INC3BUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("SRO%", INC3BUTTON_X + (AXISBUTTON_W / 2), INC3BUTTON_Y + (AXISBUTTON_H / 2));
}
