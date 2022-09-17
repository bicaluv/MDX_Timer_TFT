#include "UI.h"
#include "Debug.h"
 

#if defined(_GFXFONT_H_)           //are we using the new library?
#include <Fonts/FreeSans18pt7b.h>
#define ADJ_BASELINE 11            //new fonts setCursor to bottom of letter
#else
#define ADJ_BASELINE 0             //legacy setCursor to top of letter
#endif
#include <MCUFRIEND_kbv.h>        // https://github.com/prenticedavid/MCUFRIEND_kbv

 
#define BUTTON_X_SPACING  10
#define BUTTON_HEIGHT     50
 
// most mcufriend shields use these pins and Portrait mode:
uint8_t YP = A3;  // must be an analog pin, use "An" notation!
uint8_t XM = A2;  // must be an analog pin, use "An" notation!
uint8_t YM = 9;   // can be a digital pin
uint8_t XP = 8;   // can be a digital pin
uint16_t TS_LEFT = 906;
uint16_t TS_RT  = 149;
uint16_t TS_TOP = 913;
uint16_t TS_BOT = 142;

#define MINPRESSURE 20
#define MAXPRESSURE 1000


UI::UI() {
  _content = -1;
}

void UI::init() {
  MCUFRIEND_kbv *tft = new MCUFRIEND_kbv();
  _gfx = tft;
  
  tft->begin(0x9341);
  tft->setRotation(1); // landscape
  
  _gfx->fillScreen(BLACK);
#if defined(_GFXFONT_H_)
  _gfx->setFont(&FreeSans18pt7b);
#endif
  uint16_t tmp;
  tmp = TS_LEFT, TS_LEFT = TS_BOT, TS_BOT = TS_RT, TS_RT = TS_TOP, TS_TOP = tmp;

  // For better pressure precision, we need to know the resistance
  // between X+ and X- Use any multimeter to read it
  // For the one we're using, its 300 ohms across the X plate
  _touchScreen = new TouchScreen(XP, YP, XM, YM, 325);
}

void UI::checkButtonPressed() {  
  TSPoint touchPoint = _touchScreen->getPoint();   //touchPoint.x, touchPoint.y are ADC values
 
  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  pinMode(XP, OUTPUT);
  pinMode(YM, OUTPUT);
  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
   
  if (touchPoint.z > MINPRESSURE && touchPoint.z < MAXPRESSURE) {
    // scale from 0->1023 to _gfx->width  i.e. left = 0, rt = width
    // most mcufriend have touch (with icons) that extends below the TFT
    // screens without icons need to reserve a space for "erase"
    // scale the ADC values from _touchScreen->getPoint() to screen values e.g. 0-239
    uint16_t xpos = map(touchPoint.y, TS_LEFT, TS_RT, 0, _gfx->width()); // note x, y swapped!
    uint16_t ypos = map(touchPoint.x, TS_TOP, TS_BOT, 0, _gfx->height());
 
    for (int i = MANUAL_BUTTON; i < NUM_BUTTONS; i++) {
      if (_buttons[i].hit(xpos, ypos)) {
        break;
      }
    }
  }
}

void UI::updateContent(unsigned int newContent) {
  if (_content == newContent)
    return;

  int hh = _gfx->height() / 2;
  int hw = _gfx->width() / 2;

  // clear former content
  _gfx->fillRect(hw - 46, hh - 22, 73, 30, BLACK);

  int offset = 0;
  if (newContent < 100) {
    offset = 20;
  }
  
  _gfx->setCursor(hw - 48 + offset, hh + 5);
  _gfx->print(newContent / 10.0, 1);

  _content = newContent;
}

void UI::showSplashScreen(String name, String version) {
  _gfx->setCursor(_gfx->width() / 2 - 60, 100);
  _gfx->print(name);
  _gfx->print(" v.");
  _gfx->print(version);
}
  
void UI::addButton(int btnId, void (*callback)()) {
  _buttons[btnId].callback = callback;
}

void UI::activateButton(int btnId) {
  if (btnId == MANUAL_BUTTON) {
    _buttons[_activeButtonId].activateButtonAndDraw(false);
    
    _activeButtonId = btnId;
    _buttons[_activeButtonId].activateButtonAndDraw(true);
    
    _buttons[UP_BUTTON].showButtonAndDraw(false);
    _buttons[DOWN_BUTTON].showButtonAndDraw(false);
    return;
  }
      
  if (btnId == TIMER1_BUTTON || btnId == TIMER2_BUTTON) {
    _buttons[_activeButtonId].activateButtonAndDraw(false);
    
    _activeButtonId = btnId;
    _buttons[_activeButtonId].activateButtonAndDraw(true);

    _buttons[UP_BUTTON].showButtonAndDraw(true);
    _buttons[DOWN_BUTTON].showButtonAndDraw(true);
    return;
  }
      
  if (btnId == PLAY_BUTTON) {
    for (int i = MANUAL_BUTTON; i < NUM_BUTTONS; i++) {
      _buttons[i].enabled = true;
    }
    _buttons[PAUSE_BUTTON].enabled = _buttons[PAUSE_BUTTON].visible = false;
    _buttons[PLAY_BUTTON].visible = true;
    
    _buttons[PAUSE_BUTTON].drawButton();
    _buttons[PLAY_BUTTON].drawButton();
    return;
  }
      
  if (btnId == PAUSE_BUTTON) { 
    for (int i = MANUAL_BUTTON; i < NUM_BUTTONS; i++) {
      _buttons[i].enabled = false;
    }
    _buttons[PAUSE_BUTTON].enabled = _buttons[PAUSE_BUTTON].visible = true;
    _buttons[PLAY_BUTTON].visible = false;
    
    _buttons[PLAY_BUTTON].drawButton();
    _buttons[PAUSE_BUTTON].drawButton();
    return;
  }
      
  if (btnId == REC_BUTTON && !_buttons[REC_BUTTON].visible) {
    _buttons[REC_BUTTON].visible = true;
    _buttons[REC_BUTTON_ACTIVE].visible = false;
    
    _buttons[REC_BUTTON_ACTIVE].drawButton();
    _buttons[REC_BUTTON].drawButton();
    return;
  }
            
  if (btnId == REC_BUTTON_ACTIVE && !_buttons[REC_BUTTON_ACTIVE].visible) {
    _buttons[REC_BUTTON_ACTIVE].visible = true;
    _buttons[REC_BUTTON].visible = false;
    
    _buttons[REC_BUTTON].drawButton();
    _buttons[REC_BUTTON_ACTIVE].drawButton();
  }
}

void UI::refresh() {
  _gfx->fillScreen(BLACK);
    
  // do button layout
  int btnSize = _gfx->width() / 3 - 8;
  
  _buttons[TIMER1_BUTTON].x = 1;
  _buttons[TIMER1_BUTTON].y = 1;
  _buttons[TIMER1_BUTTON].width = btnSize;
  _buttons[TIMER1_BUTTON].height = BUTTON_HEIGHT;
  _buttons[TIMER1_BUTTON].visible = true;
  _buttons[TIMER1_BUTTON].active = false;
  _buttons[TIMER1_BUTTON].enabled = true;
  _buttons[TIMER1_BUTTON].id = TIMER1_BUTTON;
    
  _buttons[TIMER2_BUTTON].x = _buttons[TIMER1_BUTTON].x + btnSize + BUTTON_X_SPACING;
  _buttons[TIMER2_BUTTON].y = 1;
  _buttons[TIMER2_BUTTON].width = btnSize;
  _buttons[TIMER2_BUTTON].height = BUTTON_HEIGHT;
  _buttons[TIMER2_BUTTON].visible = true;
  _buttons[TIMER2_BUTTON].active = false;
  _buttons[TIMER2_BUTTON].enabled = true;
  _buttons[TIMER2_BUTTON].id = TIMER2_BUTTON;
      
  _buttons[MANUAL_BUTTON].x = _buttons[TIMER2_BUTTON].x + btnSize + BUTTON_X_SPACING;
  _buttons[MANUAL_BUTTON].y = 1;
  _buttons[MANUAL_BUTTON].width = btnSize;
  _buttons[MANUAL_BUTTON].height = BUTTON_HEIGHT;
  _buttons[MANUAL_BUTTON].visible = true;
  _buttons[MANUAL_BUTTON].active = false;
  _buttons[MANUAL_BUTTON].enabled = true;
  _buttons[MANUAL_BUTTON].id = MANUAL_BUTTON;
        
  _buttons[PLAY_BUTTON].x = _buttons[TIMER2_BUTTON].x;
  _buttons[PLAY_BUTTON].y = _gfx->height() - 1 - BUTTON_HEIGHT;
  _buttons[PLAY_BUTTON].width = btnSize;
  _buttons[PLAY_BUTTON].height = BUTTON_HEIGHT;
  _buttons[PLAY_BUTTON].visible = true;
  _buttons[PLAY_BUTTON].active = false;
  _buttons[PLAY_BUTTON].enabled = false;
  _buttons[PLAY_BUTTON].id = PLAY_BUTTON;
  
  _buttons[PAUSE_BUTTON].x = _buttons[PLAY_BUTTON].x;
  _buttons[PAUSE_BUTTON].y = _buttons[PLAY_BUTTON].y;
  _buttons[PAUSE_BUTTON].width = btnSize;
  _buttons[PAUSE_BUTTON].height = BUTTON_HEIGHT;
  _buttons[PAUSE_BUTTON].visible = false;
  _buttons[PAUSE_BUTTON].active = false;
  _buttons[PAUSE_BUTTON].enabled = false;
  _buttons[PAUSE_BUTTON].id = PAUSE_BUTTON;
  
  _buttons[REC_BUTTON].x = _buttons[PLAY_BUTTON].x - BUTTON_X_SPACING - BUTTON_HEIGHT;
  _buttons[REC_BUTTON].y = _buttons[PLAY_BUTTON].y;
  _buttons[REC_BUTTON].width = BUTTON_HEIGHT;
  _buttons[REC_BUTTON].height = BUTTON_HEIGHT;
  _buttons[REC_BUTTON].visible = false;
  _buttons[REC_BUTTON].active = false;
  _buttons[REC_BUTTON].enabled = true;
  _buttons[REC_BUTTON].id = REC_BUTTON;
    
  _buttons[REC_BUTTON_ACTIVE].x = _buttons[REC_BUTTON].x;
  _buttons[REC_BUTTON_ACTIVE].y = _buttons[REC_BUTTON].y;
  _buttons[REC_BUTTON_ACTIVE].width = BUTTON_HEIGHT;
  _buttons[REC_BUTTON_ACTIVE].height = BUTTON_HEIGHT;
  _buttons[REC_BUTTON_ACTIVE].visible = false;
  _buttons[REC_BUTTON_ACTIVE].active = false;
  _buttons[REC_BUTTON_ACTIVE].enabled = false;
  _buttons[REC_BUTTON_ACTIVE].id = REC_BUTTON_ACTIVE;
  
  _buttons[UP_BUTTON].x = _gfx->width() - BUTTON_HEIGHT;
  _buttons[UP_BUTTON].y = _gfx->height() / 2 - BUTTON_HEIGHT;
  _buttons[UP_BUTTON].width = BUTTON_HEIGHT;
  _buttons[UP_BUTTON].height = 2 * BUTTON_HEIGHT;
  _buttons[UP_BUTTON].visible = false;
  _buttons[UP_BUTTON].active = false;
  _buttons[UP_BUTTON].enabled = false;
  _buttons[UP_BUTTON].id = UP_BUTTON; 
   
  _buttons[DOWN_BUTTON].x = 0;
  _buttons[DOWN_BUTTON].y = _buttons[UP_BUTTON].y;
  _buttons[DOWN_BUTTON].width = BUTTON_HEIGHT;
  _buttons[DOWN_BUTTON].height = _buttons[UP_BUTTON].height;
  _buttons[DOWN_BUTTON].visible = false;
  _buttons[DOWN_BUTTON].active = false;
  _buttons[DOWN_BUTTON].enabled = false;
  _buttons[DOWN_BUTTON].id = DOWN_BUTTON;
  
  for (int i = MANUAL_BUTTON; i < NUM_BUTTONS; i++) {
    _buttons[i].gfx = _gfx;
    _buttons[i].drawButton();
  }
 
  _gfx->drawChar(_gfx->width() / 2 + 30, _gfx->height() / 2 + 5, 's', WHITE, BLACK, 1);
}

