/*    
   Example of drawing a "slider controller" and using
   the touch screen to change it's state (for 480x320 px).
   
   This libary introduce sevaral knob styles
   Based on the popular Bodmer's eSPI TFT Libary.
   
   Touch handling for XPT2046 based screens is handled by
   the TFT_eSPI library.

   Calibration data is stored in SPIFFS so we need to include it

   *** Created by JLGOASIS 1/08/18 ***
*/

// Calibration data is stored in SPIFFS so we need to include it
#include "FS.h"
#include <SPI.h>
#include <TFT_eSPI.h> // Hardware-specific library

#include <TFT_eSPI_TouchUI.h>

// Invoke custom library
TFT_eSPI tft = TFT_eSPI();

// This is the file name used to store the touch coordinate
// calibration data. Cahnge the name to start a new calibration.
#define CALIBRATION_FILE "/TouchCalData0"

// Set REPEAT_CAL to true instead of false to run calibration
// again, otherwise it will only be done once.
// Repeat calibration if you change the screen rotation.
#define REPEAT_CAL false

// Invoke the TFT_eSPI Slider class and create all the slider objects
TFT_eSPI_TouchUI slider[10];


uint16_t outlineColor[5] = {TFT_BLUE, TFT_WHITE, TFT_BLUE, TFT_YELLOW, TFT_YELLOW};
uint16_t fillColor[5] = {TFT_RED, TFT_GREEN, TFT_BLUE, TFT_ORANGE, TFT_BLACK};
uint16_t nofillColor[5] = {TFT_DARKGREY, TFT_BLACK, TFT_CYAN, TFT_BLACK, TFT_ORANGE};
int8_t pos[5] = {-1,0,10,15,25};

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
void setup(void) {
  Serial.begin(115200);
  Serial.println();


  tft.init();

  // Set the rotation before we calibrate
  tft.setRotation(2);

  // call screen calibration
  touch_calibrate();

  // clear screen
  tft.fillScreen(TFT_BLACK);

  tft.fillRect(0, 0, tft.width(), 20, TFT_BLUE);
  tft.setTextColor(TFT_WHITE, TFT_BLUE);
  tft.drawCentreString("SLIDER TEST", tft.width() / 2, 2, 2);

  tft.setFreeFont(&FreeSans9pt7b);

  for (uint8_t i = 0; i < 5; i++) {
    // Init parameters:
    // &tft, min value, max value, x, y , w , l,outline color, fill color, nofill color, background color, knob type, show value (-1 or < 0: No show value, 0: Automatic position, > 0: fix position) .
    slider[i].initSliderH(&tft, 0, 100, 70, 36 + i * 44, 30, 220, outlineColor[i], fillColor[i], nofillColor[i], TFT_BLACK, i, pos[i]);
    slider[i].drawSliderH(0);
    slider[i + 5].initSliderV(&tft, 0, 100, 20 + i * 60, 280, 30, 140, outlineColor[i], fillColor[i], nofillColor[i], TFT_BLACK, i, pos[i]);
    slider[i + 5].drawSliderV(0);
  }
}


//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
void loop() {
  uint16_t t_x, t_y; // To store the touch coordinates

  // Pressed will be set true is there is a valid touch on the screen
  boolean pressed = tft.getTouch(&t_x, &t_y);

  // Slider Horizontal Examples
  for (uint8_t i = 0; i < 5; i++) {
    if (pressed && slider[i].containsH(t_x, t_y)) {
      int16_t value = slider[i].getValueH(t_x); // get de x Value
      slider[i].drawSliderH(value);
    }
  }


  // Slider Vertical Examples
  for (byte i = 5; i < 10; i++) {
    if (pressed && slider[i].containsV(t_x, t_y)) {
      int16_t value = slider[i].getValueV(t_y); // get de y Value
      slider[i].drawSliderV(value);
    }
  }

}




// -----------------------------------------
void touch_calibrate() {
  uint16_t calData[5];
  uint8_t calDataOK = 0;

  // check file system exists
  if (!SPIFFS.begin()) {
    Serial.println("Formating file system");
    SPIFFS.format();
    SPIFFS.begin();
  }

  // check if calibration file exists and size is correct
  if (SPIFFS.exists(CALIBRATION_FILE)) {
    if (REPEAT_CAL) {
      // Delete if we want to re-calibrate
      SPIFFS.remove(CALIBRATION_FILE);
    }
    else
    {
      File f = SPIFFS.open(CALIBRATION_FILE, "r");
      if (f) {
        if (f.readBytes((char *)calData, 14) == 14)
          calDataOK = 1;
        f.close();
      }
    }
  }

  if (calDataOK && !REPEAT_CAL) {
    // calibration data valid
    tft.setTouch(calData);
  } else {
    // data not valid so recalibrate
    tft.fillScreen(TFT_BLACK);
    tft.setCursor(20, 0);
    tft.setTextFont(2);
    tft.setTextSize(1);
    tft.setTextColor(TFT_WHITE, TFT_BLACK);

    tft.println("Touch corners as indicated");

    tft.setTextFont(1);
    tft.println();

    if (REPEAT_CAL) {
      tft.setTextColor(TFT_RED, TFT_BLACK);
      tft.println("Set REPEAT_CAL to false to stop this running again!");
    }

    tft.calibrateTouch(calData, TFT_MAGENTA, TFT_BLACK, 15);

    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.println("Calibration complete!");

    // store data
    File f = SPIFFS.open(CALIBRATION_FILE, "w");
    if (f) {
      f.write((const unsigned char *)calData, 14);
      f.close();
    }
  }
}

