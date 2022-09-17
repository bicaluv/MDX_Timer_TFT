#ifndef UI_h
#define UI_h

#include <Arduino.h>
#include <Adafruit_GFX.h>     // Core graphics library: https://github.com/adafruit/Adafruit-GFX-Library
#include <TouchScreen.h>      // https://github.com/adafruit/Touch-Screen-Library

#include "Button.h"


#define NUM_BUTTONS        9

#define MANUAL_BUTTON      0
#define TIMER1_BUTTON      1
#define TIMER2_BUTTON      2
#define PLAY_BUTTON        3
#define REC_BUTTON         4
#define UP_BUTTON          5
#define DOWN_BUTTON        6
#define REC_BUTTON_ACTIVE  7
#define PAUSE_BUTTON       8


class UI {
public:
  UI();

  void init();
  void refresh();
    
  void showSplashScreen(String name, String version);
  void updateContent(unsigned int newContent);
  
  void checkButtonPressed();
  void addButton(int btnId, void (*callback)());
  void activateButton(int btnId);
  
private:  
  int _activeButtonId;
  unsigned int _content;
  Button _buttons[NUM_BUTTONS];

  Adafruit_GFX *_gfx;
  TouchScreen *_touchScreen;
};

#endif
