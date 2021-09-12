void getTouchMPGPage() {
  uint16_t x, y;

  if (tft.getTouch(&x, &y)) {
    screenTime = millis();
    
    if (!screenActive) {
      screenActive = true;
      digitalWrite(SCRLED, HIGH);
    }
     
    if ((x > XAXISBUTTON_X) && (x < (XAXISBUTTON_X + AXISBUTTON_W))) {
      if ((y > XAXISBUTTON_Y) && (y <= (XAXISBUTTON_Y + AXISBUTTON_H))) {
        selectAxis(XAXISBUTTON_X, XAXISBUTTON_Y, "X");
        mb.Coil(regs[lastAxis], 0);
        lastAxis = 0;
        mb.Coil(regs[0], 1);
      }
    }

    if ((x > YAXISBUTTON_X) && (x < (YAXISBUTTON_X + AXISBUTTON_W))) {
      if ((y > YAXISBUTTON_Y) && (y <= (YAXISBUTTON_Y + AXISBUTTON_H))) {
        selectAxis(YAXISBUTTON_X, YAXISBUTTON_Y, "Y");
        mb.Coil(regs[lastAxis], 0);
        lastAxis = 1;
        mb.Coil(regs[1], 1);
      }
    }

    if ((x > ZAXISBUTTON_X) && (x < (ZAXISBUTTON_X + AXISBUTTON_W))) {
      if ((y > ZAXISBUTTON_Y) && (y <= (ZAXISBUTTON_Y + AXISBUTTON_H))) {
        selectAxis(ZAXISBUTTON_X, ZAXISBUTTON_Y, "Z");
        mb.Coil(regs[lastAxis], 0);
        lastAxis = 2;
        mb.Coil(regs[2], 1);
      }
    }

    if ((x > AAXISBUTTON_X) && (x < (AAXISBUTTON_X + AXISBUTTON_W))) {
      if ((y > AAXISBUTTON_Y) && (y <= (AAXISBUTTON_Y + AXISBUTTON_H))) {
        selectAxis(AAXISBUTTON_X, AAXISBUTTON_Y, "A");
        mb.Coil(regs[lastAxis], 0);
        lastAxis = 3;
        mb.Coil(regs[3], 1);
      }
    }

    if ((x > BAXISBUTTON_X) && (x < (BAXISBUTTON_X + AXISBUTTON_W))) {
      if ((y > BAXISBUTTON_Y) && (y <= (BAXISBUTTON_Y + AXISBUTTON_H))) {
        selectAxis(BAXISBUTTON_X, BAXISBUTTON_Y, "B");
        mb.Coil(regs[lastAxis], 0);
        lastAxis = 4;
        mb.Coil(regs[4], 1);
      }
    }

    if ((x > CAXISBUTTON_X) && (x < (CAXISBUTTON_X + AXISBUTTON_W))) {
      if ((y > CAXISBUTTON_Y) && (y <= (CAXISBUTTON_Y + AXISBUTTON_H))) {
        selectAxis(CAXISBUTTON_X, CAXISBUTTON_Y, "C");
        mb.Coil(regs[lastAxis], 0);
        lastAxis = 5;
        mb.Coil(regs[5], 1);
      }
    }

    if ((x > INC1BUTTON_X) && (x < (INC1BUTTON_X + AXISBUTTON_W))) {
      if ((y > INC1BUTTON_Y) && (y <= (INC1BUTTON_Y + AXISBUTTON_H))) {
        selectInc(INC1BUTTON_X, INC1BUTTON_Y, "0.001");
        mb.Coil(regs[lastInc], 0);
        lastInc = 7;
        mb.Coil(regs[7], 1);
      }
    }

    if ((x > INC2BUTTON_X) && (x < (INC2BUTTON_X + AXISBUTTON_W))) {
      if ((y > INC2BUTTON_Y) && (y <= (INC2BUTTON_Y + AXISBUTTON_H))) {
        selectInc(INC2BUTTON_X, INC2BUTTON_Y, "0.01");
        mb.Coil(regs[lastInc], 0);
        lastInc = 8;
        mb.Coil(regs[8], 1);
      }
    }

    if ((x > INC3BUTTON_X) && (x < (INC3BUTTON_X + AXISBUTTON_W))) {
      if ((y > INC3BUTTON_Y) && (y <= (INC3BUTTON_Y + AXISBUTTON_H))) {
        selectInc(INC3BUTTON_X, INC3BUTTON_Y, "0.1");
        mb.Coil(regs[lastInc], 0);
        lastInc = 9;
        mb.Coil(regs[9], 1);
      }
    }
  
    if ((x > BACKBUTTON_X) && (x < (BACKBUTTON_X + AXISBUTTON_W))) {
      if ((y > BACKBUTTON_Y) && (y <= (BACKBUTTON_Y + AXISBUTTON_H))) {
        drawDROPage();
      }
    }
  }
}

void drawMPGPage() {
  pageNum = 3;

  tft.fillScreen(TFT_BLACK);
  drawAxisButtons();
  drawIncButtons();
  drawBackButton();

  switch(lastAxis) {
    case 0:
      selectAxis(XAXISBUTTON_X, XAXISBUTTON_Y, "X");
      break;
    case 1:
      selectAxis(YAXISBUTTON_X, YAXISBUTTON_Y, "Y");
      break;
    case 2:
      selectAxis(ZAXISBUTTON_X, ZAXISBUTTON_Y, "Z");
      break;
    case 3:
      selectAxis(AAXISBUTTON_X, AAXISBUTTON_Y, "A");
      break;
    case 4:
      selectAxis(BAXISBUTTON_X, BAXISBUTTON_Y, "B");
      break;
    case 5:
      selectAxis(CAXISBUTTON_X, CAXISBUTTON_Y, "C");
      break;
  }

  switch(lastInc) {
    case 7:
      selectInc(INC1BUTTON_X, INC1BUTTON_Y, "0.001");
      break;
    case 8:
      selectInc(INC2BUTTON_X, INC2BUTTON_Y, "0.01");
      break;
    case 9:
      selectInc(INC3BUTTON_X, INC3BUTTON_Y, "0.1");
      break;
  }
}

void drawIncButtons() {
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("MPG Increment:", 125, 170);
  
  tft.fillRect(INC1BUTTON_X, INC1BUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("0.001", INC1BUTTON_X + (AXISBUTTON_W / 2), INC1BUTTON_Y + (AXISBUTTON_H / 2));
  
  tft.fillRect(INC2BUTTON_X, INC2BUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("0.01", INC2BUTTON_X + (AXISBUTTON_W / 2), INC2BUTTON_Y + (AXISBUTTON_H / 2));
  
  tft.fillRect(INC3BUTTON_X, INC3BUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("0.1", INC3BUTTON_X + (AXISBUTTON_W / 2), INC3BUTTON_Y + (AXISBUTTON_H / 2));
}

void drawAxisButtons() {
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("MPG Axis:", 120, 20);
  
  tft.fillRect(XAXISBUTTON_X, XAXISBUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("X", XAXISBUTTON_X + (AXISBUTTON_W / 2), XAXISBUTTON_Y + (AXISBUTTON_H / 2)); 
  
  tft.fillRect(YAXISBUTTON_X, YAXISBUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Y", YAXISBUTTON_X + (AXISBUTTON_W / 2), YAXISBUTTON_Y + (AXISBUTTON_H / 2)); 
  
  tft.fillRect(ZAXISBUTTON_X, ZAXISBUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("Z", ZAXISBUTTON_X + (AXISBUTTON_W / 2), ZAXISBUTTON_Y + (AXISBUTTON_H / 2)); 
  
  tft.fillRect(AAXISBUTTON_X, AAXISBUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("A", AAXISBUTTON_X + (AXISBUTTON_W / 2), AAXISBUTTON_Y + (AXISBUTTON_H / 2)); 
  
  tft.fillRect(BAXISBUTTON_X, BAXISBUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("B", BAXISBUTTON_X + (AXISBUTTON_W / 2), BAXISBUTTON_Y + (AXISBUTTON_H / 2)); 
  
  tft.fillRect(CAXISBUTTON_X, CAXISBUTTON_Y, AXISBUTTON_W, AXISBUTTON_H, TFT_DARKGREY);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString("C", CAXISBUTTON_X + (AXISBUTTON_W / 2), CAXISBUTTON_Y + (AXISBUTTON_H / 2));
}

void selectAxis(int32_t x, int32_t y, const char *axisID) {
  drawAxisButtons();
  tft.fillRect(x, y, AXISBUTTON_W, AXISBUTTON_H, TFT_GREEN);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(axisID, x + (AXISBUTTON_W / 2), y + (AXISBUTTON_H / 2));
}

void selectInc(int32_t x, int32_t y, const char *axisID) {
  drawIncButtons();
  tft.fillRect(x, y, AXISBUTTON_W, AXISBUTTON_H, TFT_GREEN);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextDatum(MC_DATUM);
  tft.drawString(axisID, x + (AXISBUTTON_W / 2), y + (AXISBUTTON_H / 2));
}
