/***************************************************************************************
// The following button class has been ported over from the Adafruit_GFX library so
// should be compatible.
// A slightly different implementation in this TFT_eSPI library allows the button
// legends to be in any font
***************************************************************************************/

#ifndef TFT_eSPI_TouchUI_h
 #define TFT_eSPI_TouchUI_h
 #include <TFT_eSPI_TouchUI.h>
 #include <TFT_eSPI.h>


class TFT_eSPI_TouchUI {

 public:
  TFT_eSPI_TouchUI(void);
  
  // *** Public Button ***
  // "Classic" initButton() uses center & size
  void     initButton(TFT_eSPI *gfx, int16_t x, int16_t y,
  uint16_t w, uint16_t h, uint16_t outline, uint16_t oncolor,
  uint16_t offcolor, char *label, uint8_t knob);

  // New/alt initButton() uses upper-left corner & size
  void     initButtonUL(TFT_eSPI *gfx, int16_t x1, int16_t y1,
  uint16_t w, uint16_t h, uint16_t outline, uint16_t oncolor,
  uint16_t offcolor, char *label, uint8_t knob);

  void     drawButton(boolean inverted = false);
  
  // *** Public Graphic Button ***
  // initButton() uses center & size
  void     initButtonG(TFT_eSPI *gfx, int16_t x, int16_t y,
  uint16_t w, uint16_t h, uint16_t outline, uint16_t oncolor,
  uint16_t offcolor, char *image, uint8_t knob);

  // New/alt initButton() uses upper-left corner & size
  void     initButtonGUL(TFT_eSPI *gfx, int16_t x1, int16_t y1, uint16_t w, uint16_t h, uint16_t outline, uint16_t oncolor, uint16_t offcolor, char *image, uint8_t knob);
  void     drawButtonG(boolean inverted = false);
  
// *** Helper Button ***
  boolean  contains(int16_t x, int16_t y);
  void     press(boolean p);
  boolean  isPressed();
  boolean  justPressed();
  boolean  justReleased();



  // *** Public Slider Button ***
  void initButtonS(TFT_eSPI *gfx, int16_t x, int16_t y, uint16_t w, uint16_t l, uint16_t outline, uint16_t oncolor, uint16_t offcolor, uint8_t knob, boolean lb);

  void drawButtonS(boolean inverted = false);

  boolean  justPressedOn(int16_t x);
  boolean  justPressedOff(int16_t x);

  // *** Public Slider ***
  // Init the Horizontal Slider
    void initSliderH(TFT_eSPI *gfx, int16_t mim1, int16_t max1, int16_t x, int16_t y,	uint16_t w, uint16_t l, uint16_t outline, uint16_t fill, uint16_t nofill, uint8_t knob, int8_t dv);
    void drawSliderH(int16_t x);
    boolean containsH(int16_t x, int16_t y);
    int16_t getValueH(int16_t v);

    // Init the Vertical Slider
    void initSliderV(TFT_eSPI *gfx, int16_t mim1, int16_t max1, int16_t x, int16_t y,	uint16_t w, uint16_t l, uint16_t outline, uint16_t fill, uint16_t nofill, uint8_t knob, int8_t dv);

    void drawSliderV(int16_t x);
    boolean containsV(int16_t x, int16_t y);
    int16_t getValueV(int16_t v);

 private:
  TFT_eSPI *_gfx;

  // *** Privates Button ***
  int16_t  _x, _y; 		// Coordinates of top-left corner for button & slider
  uint16_t _w, _h, _l;		// h for botton, l for slider
  uint8_t  _knobstyle;
  uint16_t _outlinecolor, _oncolor, _offcolor,_fillcolor, _nofillcolor, _bgcolor;
  char     _label[10];		// Button label
  char     _image[10];		// Graphic Button image label
  boolean  _labelOn;		// Label ON / OFF for Slider Button

  boolean  currstate, laststate; // for buttons

  int16_t   _min, _max; 	// min and max value for Slider
  int8_t    _drawvalue;		// Draw value for slider
  int16_t   _oldvalue=-999;	// determinate de last position of knob for Slider

};

#endif
