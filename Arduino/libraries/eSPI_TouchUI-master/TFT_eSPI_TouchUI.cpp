/***************************************************************************************
** Code for the GFX button UI element
** Grabbed from Adafruit_GFX library and enhanced to handle any label font
***************************************************************************************/


#include <TFT_eSPI_TouchUI.h>

TFT_eSPI_TouchUI::TFT_eSPI_TouchUI(void) {
  _gfx = 0;
}


/****************************************************
***************   BUTTON CLASS **********************
*****************************************************/

// *** Init  Button *** pass center & size
void TFT_eSPI_TouchUI::initButton(
 TFT_eSPI *gfx, int16_t x, int16_t y, uint16_t w, uint16_t h,
 uint16_t outline, uint16_t oncolor, uint16_t offcolor,
 char *label, uint8_t knob)
{
  // Tweak arguments and pass to the newer initButtonUL() function...
  initButtonUL(gfx, x - (w / 2), y - (h / 2), w, h, outline, oncolor,
    offcolor, label, knob);
}

// Newer function instead accepts upper-left corner & size
void TFT_eSPI_TouchUI::initButtonUL(
 TFT_eSPI *gfx, int16_t x1, int16_t y1, uint16_t w, uint16_t h,
 uint16_t outline, uint16_t oncolor, uint16_t offcolor,
 char *label, uint8_t knob)
{
  _gfx          = gfx;

  _x            = x1;
  _y            = y1;
  _w            = w;
  _h            = h;

  _outlinecolor = outline;
  _oncolor      = oncolor;
  _offcolor     = offcolor;
  _knobstyle    = knob;

  _bgcolor      = _gfx->readPixel(x1, y1); // Get de background color

  strncpy(_label, label, 9);
}

// *** Draw  Button ***
void TFT_eSPI_TouchUI::drawButton(boolean inverted) {
  uint16_t outline, oncolor, offcolor, ledcolor;
  uint16_t r = _w / 2;
  uint16_t x2 = _x + r;
  uint16_t y2 = _y + r;
  uint16_t x3 = _x + _w / 2;  
  uint16_t y3 = _y + _h / 2;
  uint16_t xt = _x + _w / 2;
  uint16_t yt = _y + _h / 2;
  uint8_t  fl = min(_w, _h) / 12;  // fill offset for cicle

  if(!inverted) {
    oncolor    = _oncolor;
    offcolor   = _offcolor;
    ledcolor   = _bgcolor;
  } else {
    oncolor    = _offcolor;
    offcolor   = _oncolor;
    ledcolor   = _oncolor;
  }

  // Draw Knob
    switch (_knobstyle) {
     case 1:   // Rectangular Rounded knob. Original from Button lib
    	r = min(_w, _h) / 4;  // Corner radius
    	_gfx->fillRoundRect(_x, _y, _w, _h, r, oncolor);
    	_gfx->drawRoundRect(_x, _y, _w, _h, r, _outlinecolor);
        break;

      case 2:   // Circular or Rounded Knob
	if (_w >= _h) {
	 // _gfx->fillRoundRect(_x, _y, _w, _h, _h/2, _bgcolor);
	  _gfx->fillRoundRect(_x + fl, _y + fl, _w - fl * 2, _h - fl * 2, _h/2 - fl, oncolor);
	  _gfx->drawRoundRect(_x, _y, _w, _h, _h/2, _outlinecolor);
        }
	if (_w < _h) {
	  // _gfx->fillRoundRect(_x, _y, _w, _h, _w/2, _bgcolor);
	  _gfx->fillRoundRect(_x + fl, _y + fl, _w - fl * 2, _h - fl * 2, _w/2 - fl, oncolor);
	  _gfx->drawRoundRect(_x, _y, _w, _h, _w/2, _outlinecolor);
	}
	
/*
  	// _gfx->fillCircle(x2, y2, r, _bgcolor);
    	_gfx->fillCircle(x2 , y2, r - fl, oncolor);
    	_gfx->drawCircle(x2, y2, r, _outlinecolor);
*/
        break;

      case 3:   // Rectangular Rounded knob with led
    	r = min(_w, _h) / 4;  // Corner radius
    	yt = _y + _h / 1.8;
        _gfx->fillRoundRect(_x, _y, _w, _h, r, _bgcolor);
    	_gfx->drawRoundRect(x3 - (_w/6),     _y + fl,     _w/3,     _h/6,     r/4, _outlinecolor);  // led line
        _gfx->fillRoundRect(x3 - (_w/6) + 1, _y + fl + 1, _w/3 - 2, _h/6 - 2, r/4, ledcolor);  // led color
    	_gfx->drawRoundRect(_x, _y, _w, _h, r, _outlinecolor);
        break;

      case 4:   // Triangle Knob Up
  	yt = _y +_h / 1.5;
        _gfx->fillTriangle(_x + r, _y, _x +_w, _y + _h, _x, _y + _h, oncolor);
        //_gfx->fillTriangle(_x + r, _y + fly, _x +_w - flx, _y + _h - fly, _x + flx, _y + _h - fly, oncolor);
  	_gfx->drawTriangle(_x + r, _y, _x +_w, _y + _h, _x, _y + _h, _outlinecolor);
        break;

      case 5:   // Triangle Knob Right
  	xt = _x +_w/3;
        _gfx->fillTriangle(_x, _y, _x + _w, y3, _x, _y + _h, oncolor);
        _gfx->drawTriangle(_x, _y, _x + _w, y3, _x, _y + _h, _outlinecolor);
        break;

       case 6:   // Triangle Knob Down
  	yt = _y + _h/3;
        _gfx->fillTriangle(_x, _y, _x + _w, _y, x3, _y + _h, oncolor);
  	_gfx->drawTriangle(_x, _y, _x + _w, _y, x3, _y + _h, _outlinecolor);
        break;

      case 7:   // Triangle Knob Left
  	xt = _x + _w/1.5;
        _gfx->fillTriangle(_x, y3, _x + _w, _y, _x + _w, _y + _h, oncolor);
  	_gfx->drawTriangle(_x, y3, _x + _w, _y, _x + _w, _y + _h, _outlinecolor);
        break;

      default: // Rectangular knob.
  	// _gfx->fillRect(_x, _y, _w, _h, _bgcolor);
    	_gfx->fillRect(_x + fl, _y + fl, _w - fl * 2, _h - fl * 2, oncolor);
    	_gfx->drawRect(_x, _y, _w, _h, _outlinecolor);
      	break;
    }

  // Draw Label
  _gfx->setTextColor(offcolor);
  // _gfx->setTextSize(_textsize);

  uint8_t tempdatum = _gfx->getTextDatum();
  _gfx->setTextDatum(MC_DATUM);
  _gfx->drawString(_label, xt, yt);
  _gfx->setTextDatum(tempdatum);
}



/****************************************************
***************   GRAPHIC BUTTON CLASS **************
*****************************************************/

// *** Init  Button *** pass center & size
void TFT_eSPI_TouchUI::initButtonG(
 TFT_eSPI *gfx, int16_t x, int16_t y, uint16_t w, uint16_t h,
 uint16_t outline, uint16_t oncolor, uint16_t offcolor,
 char *image, uint8_t knob)
{
  // Tweak arguments and pass to the newer initButtonUL() function...
  initButtonGUL(gfx, x - (w / 2), y - (h / 2), w, h, outline, oncolor,
    offcolor, image, knob);
}

// Newer function instead accepts upper-left corner & size
void TFT_eSPI_TouchUI::initButtonGUL(
 TFT_eSPI *gfx, int16_t x1, int16_t y1, uint16_t w, uint16_t h,
 uint16_t outline, uint16_t oncolor, uint16_t offcolor,
 char *image, uint8_t knob)
{
  _gfx          = gfx;

  _x            = x1;
  _y            = y1;
  _w            = w;
  _h            = h;

  _outlinecolor = outline;
  _oncolor      = oncolor;
  _offcolor     = offcolor;
  _knobstyle    = knob;

  _bgcolor      = _gfx->readPixel(x1, y1); // Get de background color

  strncpy(_image, image, 9);
}

// *** Draw  Button ***
void TFT_eSPI_TouchUI::drawButtonG(boolean inverted) {
  uint16_t outline, oncolor, offcolor, ledcolor;
  uint16_t r = _h / 2;

  if(!inverted) {
    oncolor    = _oncolor;
  } else {
    oncolor    = _offcolor;
  }

  // Draw Knob
    switch (_knobstyle) {

      case 1: // Rectangular outline
	_gfx->drawBitmap(_x, _y, (const uint8_t*)(& _image), _w, _h, oncolor);
    	_gfx->drawRect(_x, _y, _w, _h, _outlinecolor);
      	break;

     case 2:   // Rectangular Rounded outline
	_gfx->drawBitmap(_x, _y, (const uint8_t*)(& _image), _w, _h, oncolor);	
    	r = min(_w, _h) / 4;  // Corner radius
    	_gfx->drawRoundRect(_x, _y, _w, _h, r, _outlinecolor);
        break;

      case 3:   // Circular outline 
	_gfx->drawBitmap(_x, _y, (const uint8_t*)(& _image), _w, _h, oncolor);	
    	_gfx->drawCircle(_x + r, _y + r, r, _outlinecolor);
        break;

      default: // No outline
	_gfx->drawBitmap(_x, _y, (const uint8_t*)(& _image), _w, _h, oncolor);	
      	break;
    }

}



/****************************************************
***************   SLIDER BUTTON CLASS ***************
*****************************************************/

// *** Init  Slider Button ***
void TFT_eSPI_TouchUI::initButtonS(TFT_eSPI *gfx, int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t outline, uint16_t oncolor, uint16_t offcolor, uint8_t knob, boolean lb) {
  _gfx          = gfx;

  _x            = x;
  _y            = y;
  _w            = w;
  _h            = h;

  _outlinecolor = outline;
  _oncolor      = oncolor;
  _offcolor     = offcolor;

 // _bgcolor      = _gfx->readPixel(x,y); // Get de background color
  _knobstyle    = knob;
  _labelOn      = lb;

}


void TFT_eSPI_TouchUI::drawButtonS(boolean inverted) {
  uint16_t oncolor, offcolor;
  uint16_t w2 = _x + _w / 2;
  uint16_t w3 = w2 / 16;
  uint16_t fl = _h / 15;
  uint16_t r = _h / 2;
  uint16_t xt1 = _x + _w / 4;
  uint16_t xt2 = _x + _w - _w / 4;
  uint16_t yt = _y + r;

  if(inverted) {
    oncolor        = _oncolor;
    offcolor       = _outlinecolor;
  } else {
    oncolor        = _outlinecolor;
    offcolor       = _offcolor;
  }


  // Draw
    switch (_knobstyle) {
     case 1:   // Triangle Button
  	_gfx->fillRect(_x+fl, _y+fl, _w/2-fl, _h-fl*2, offcolor);
	_gfx->fillRect(w2, _y+fl, _w/2-fl, _h-fl*2, oncolor);
	_gfx->fillTriangle (w2-w3, _y+fl, w2+w3, _y+fl, w2-w3, _y + _h-fl-1, offcolor);
	_gfx->fillTriangle (w2+w3, _y+fl, w2+w3, _y + _h-fl-1, w2-w3, _y + _h-fl-1, oncolor);
  	_gfx->drawRect(_x, _y, _w, _h, _outlinecolor);
      	break;

     case 2:   // Rounded button
  	_gfx->fillRoundRect(_x, _y, _w, _h, r, _outlinecolor);
  	_gfx->fillCircle(_x + r + fl , yt, r - fl, offcolor);
  	_gfx->fillCircle(_x + _w - r - fl , yt, r - fl, oncolor);
      	break;

      default: // Rectangular button
  	_gfx->fillRect(_x+fl, _y+fl, _w / 2-fl, _h-fl*2, offcolor);
  	_gfx->fillRect(w2, _y+fl, _w / 2-fl, _h-fl*2, oncolor);
  	_gfx->drawRect(_x, _y, _w, _h, _outlinecolor);
      	break;
    }
  
  // Draw Label ON/OFF
  if (_labelOn) {
  	uint8_t tempdatum = _gfx->getTextDatum();
  	_gfx->setTextDatum(MC_DATUM);
  	_gfx->setTextColor(_offcolor);
  	_gfx->drawString("OFF", xt1, yt);
  	_gfx->setTextColor(_oncolor);
  	_gfx->drawString("ON", xt2, yt);
  	_gfx->setTextDatum(tempdatum);
 }

}

boolean TFT_eSPI_TouchUI::justPressedOn(int16_t x) { return (currstate && !laststate && x > _x + _w/2);  }

boolean TFT_eSPI_TouchUI::justPressedOff(int16_t x) { return (currstate && !laststate && x < _x + _w/2);  }



/***************** helper button & slider button *****************************/

// *** Contains  Button ***
boolean TFT_eSPI_TouchUI::contains(int16_t x, int16_t y) {
  return ((x >= _x) && (x < (_x + _w)) &&
          (y >= _y) && (y < (_y + _h)));
}

// *** Contains  Button ***
void TFT_eSPI_TouchUI::press(boolean p) {
  laststate = currstate;
  currstate = p;
}


boolean TFT_eSPI_TouchUI::isPressed()    { return currstate; }
boolean TFT_eSPI_TouchUI::justPressed()  { return (currstate && !laststate); }
boolean TFT_eSPI_TouchUI::justReleased() { return (!currstate && laststate); }


/****************************************************
***************   SLIDER CLASS **********************
*****************************************************/


// *** Init Horizontal Slider ***
void TFT_eSPI_TouchUI::initSliderH(TFT_eSPI *gfx, int16_t min1, int16_t max1, int16_t x, 
  int16_t y, uint16_t w, uint16_t l, uint16_t outline, uint16_t fill, uint16_t nofill, uint8_t knob, int8_t dv) {
  _gfx          = gfx;

  _min         = min1;
  _max         = max1;

  _x            = x;
  _y            = y;
  _w            = w;
  _l            = l;

  _outlinecolor = outline;
  _fillcolor    = fill;
  _nofillcolor  = nofill;

  _bgcolor      = _gfx->readPixel(_x, _y); // Get de background color

  _knobstyle    = knob;
  _drawvalue    = dv;

}


// *** Draw Horizontal Slider ***
void TFT_eSPI_TouchUI::drawSliderH(int16_t value) {
  value = constrain(value, _min, _max);
  if (_oldvalue != value) {
    int16_t value1 = map(value, _min, _max, 0, _l);
    int16_t x2 = _x + value1;     // X Value Point
    int16_t y2 = _y + (_w / 2);   // Y Mid Point
    uint8_t g = _w;               // Gap width
    uint8_t r = _w / 2;           // Knob circular radio
    uint8_t h = 0;                // Knob rectangular height, also determinate text position

    // Clear old position except knob 0
    if (_knobstyle != 0) {
      h = _w; 
      g = 10;
      if (_knobstyle == 2) h = _w * 1.8; // For rectangular knob
      _gfx->fillRect(_x + _oldvalue - (h / 2), _y, h + 1, _w + 1, _bgcolor); // Fill old Position
    }

    // Draw Gap
    _gfx->fillRect(_x, y2 - (g / 2), value1, g, _fillcolor);            // Fill Value
    _gfx->fillRect(x2, y2 - (g / 2), _l - value1, g, _nofillcolor);     // Fill Rest
    _gfx->drawRect(_x, y2 - (g / 2), _l, g, _outlinecolor);
    _oldvalue = value1;

    // Draw Knob
    switch (_knobstyle) {
      case 1:   // Circular knob
        _gfx->fillRect(x2 - r, _y, _w, _w, _bgcolor);
        _gfx->drawCircle(x2, y2, r, _outlinecolor);
        _gfx->fillCircle(x2, y2, r - 4, _fillcolor);
        break;

      case 2:   // Rectangular Knob
        _gfx->fillRect(x2 - (h / 2), _y, h,_w, _bgcolor);
        _gfx->drawRoundRect(x2 - (h / 2), _y, h, _w, _w / 4, _outlinecolor);
  	_gfx->drawFastVLine(x2, _y + 8, _w - 16, _outlinecolor);
  	_gfx->drawFastVLine(x2-(h/10), _y + 8, _w - 16, _outlinecolor);
  	_gfx->drawFastVLine(x2+(h/10), _y + 8, _w - 16, _outlinecolor);
        break;

      case 3:   // Triangle Knob Down
        _gfx->fillTriangle(x2, _y + _w, x2 - r, _y, x2 + r, _y, _bgcolor);
  	_gfx->drawTriangle(x2, _y + _w, x2 - r, _y, x2 + r, _y, _outlinecolor);
        break;

      case 4:   // Triangle Knob Up
        _gfx->fillTriangle(x2, _y, x2 - r, _y + _w, x2 + r, _y + _w, _bgcolor);
  	_gfx->drawTriangle(x2, _y, x2 - r, _y + _w, x2 + r, _y + _w, _outlinecolor);
        break;

      default: 
      // if nothing else matches, do the default
      break;
    }

    // Draw Value

    if (_drawvalue >= 0) {
      static uint8_t tempdatum = _gfx->getTextDatum();
      _gfx->setTextDatum(MR_DATUM);
      _gfx->setTextColor(TFT_WHITE, _bgcolor);

      uint8_t padding =_gfx->textWidth(String(_max)+"-");

      // uint8_t padding = max(_gfx->textWidth(String(_min)),_gfx->textWidth(String(_max)));  // max padding
      _gfx->setTextPadding(padding);

      int8_t o = 6;   // Offset position value
      if (_drawvalue > 0) o = _drawvalue;      // ¿¿¿

      _gfx->drawNumber(value, _x - h / 2 - o, y2); 
      _gfx->setTextPadding(0);
      _gfx->setTextDatum(tempdatum);

    }
  }
}


// *** Slide Constains Vertical ***
boolean TFT_eSPI_TouchUI::containsH(int16_t x, int16_t y) {
  return ((x >= _x) && (x <= (_x + _l)) && (y >= _y) && (y <= (_y + _w)));
}


// *** Get Slide Value, X coordinate  for Horizontal Slide ***
int16_t TFT_eSPI_TouchUI::getValueH(int16_t v) {
  int16_t v1 = constrain(v, _x, _x + _l);
  int16_t value = map(v1, _x, _x + _l, _min, _max);
  return value;
}


/*****************************************************************************************/

// *** Init Vertical Slider ***
void TFT_eSPI_TouchUI::initSliderV(
  TFT_eSPI *gfx, int16_t min1, int16_t max1, int16_t x, int16_t y,
  uint16_t w, uint16_t l, 
  uint16_t outline, uint16_t fill, uint16_t nofill, uint8_t knob, int8_t dv) {
  _gfx          = gfx;

  _min         = min1;
  _max         = max1;

  _x            = x;
  _y            = y;
  _w            = w;
  _l            = l;

  _outlinecolor = outline;
  _fillcolor    = fill;
  _nofillcolor  = nofill;
 // _bgcolor      = bg;
  _bgcolor      = _gfx->readPixel(x,y); // Get de background color

  _knobstyle    = knob;
  _drawvalue   = dv;
}


// *** Draw Vertical Slider ***
void TFT_eSPI_TouchUI::drawSliderV(int16_t value) {
  value = constrain(value, _min, _max);
  if (_oldvalue != value) {
    int16_t value1 = map(value, _min, _max, 0, _l);
    int16_t x2 = _x + (_w / 2);    // X Mid Point
    int16_t y2 = (_y + _l) - value1; // Y Value Point
    uint8_t g = _w;                // Gap width
    uint8_t r = _w / 2;            // Knob circular radio
    uint8_t h = 0;             // Knob rectangular height also determinate text position
  
    // Clear old position except knob 0
    if (_knobstyle != 0) {
      h = _w;   // 0 position for text  
      g = 10;
      if (_knobstyle == 2) h = _w * 1.8; // For rectangular knob
      _gfx->fillRect(_x, (_y + _l) - _oldvalue - (h / 2), _w + 1, h + 1, _bgcolor); // Fill old Position
    }

    // Draw Gap
    _gfx->fillRect(x2 - (g / 2), y2, g, value1, _fillcolor);      // Fill Value
    _gfx->fillRect(x2 - (g / 2), _y, g, _l - value1, _nofillcolor); // Fill Rest
    _gfx->drawRect(x2 - (g / 2), _y, g, _l, _outlinecolor);
    _oldvalue = value1;

    // Draw Knob
    switch (_knobstyle) {
      case 1:   // Circular knob
        _gfx->fillRect(_x, y2 - r, _w, _w, _bgcolor);
        _gfx->drawCircle(x2, y2, r, _outlinecolor);
        _gfx->fillCircle(x2, y2, r - 4, _fillcolor);
        break;

      case 2:   // Rectangular Knob
        _gfx->fillRect(_x, y2 - (h / 2), _w, h, _bgcolor);
        _gfx->drawRoundRect(_x, y2 - (h / 2), _w, h, _w / 4, _outlinecolor);
  	_gfx->drawFastHLine(_x + 8, y2, _w - 16, _outlinecolor);
  	_gfx->drawFastHLine(_x + 8, y2 - (h/10), _w - 16, _outlinecolor);
  	_gfx->drawFastHLine(_x + 8, y2 + (h/10), _w - 16, _outlinecolor);
        break;

      case 3:   // Triangle Knob Right
        _gfx->fillTriangle(_x + _w, y2, _x, y2 + r, _x, y2 - r, _bgcolor);
  	_gfx->drawTriangle(_x + _w, y2, _x, y2 + r, _x, y2 - r, _outlinecolor);
        break;

      case 4:   // Triangle Knob Left
        _gfx->fillTriangle(_x, y2, _x + _w, y2 - r, _x + _w, y2 + r, _bgcolor);
  	_gfx->drawTriangle(_x, y2, _x + _w, y2 - r, _x + _w, y2 + r, _outlinecolor);
        break;

      default: 
      // if nothing else matches, do the default
      break;
    }

    // Draw Value

    if (_drawvalue >= 0) {

      static uint8_t tempdatum = _gfx->getTextDatum();
      _gfx->setTextColor(TFT_WHITE, _bgcolor);
      _gfx->setTextPadding(_gfx->textWidth(String(_max)+"-"));  // include - symbol for negative values.  Mejorar
      _gfx->setTextDatum(TC_DATUM);
      int8_t o = 6;   // Offset position value
      if (_drawvalue > 0) o = _drawvalue;

      _gfx->drawNumber(value, x2, _y + _l + h/2 + o); 
      _gfx->setTextPadding(0);
      _gfx->setTextDatum(tempdatum);
    }
  }
}


// *** Slide Constains Vertical ***
boolean TFT_eSPI_TouchUI::containsV(int16_t x, int16_t y) {
  return ((x >= _x) && (x <= (_x + _w)) && (y >= _y) && (y <= (_y + _l)));
}


// *** Get Slide Value, X coordinate  for Horizontal Slide ***
int16_t TFT_eSPI_TouchUI::getValueV(int16_t v) {
  int16_t v1 = constrain(v, _y, _y + _l);
  int16_t value = map(v1, _y, _y + _l, _max, _min);
  return value;
}

