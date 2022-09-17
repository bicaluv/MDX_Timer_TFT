#ifndef Button_h
#define Button_h

#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library: https://github.com/adafruit/Adafruit-GFX-Library

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define RED     0xF800
#define GREEN   0x07E0
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
#define GRAY    0x8410


class Button {
public:
  Button();
  
  void drawButton();

  bool hit(uint16_t xpos, uint16_t ypos);
  
  void showButtonAndDraw(bool isVisible);
  void activateButtonAndDraw(bool activate);

  void (*callback)();

  Adafruit_GFX *gfx;
  
  unsigned int x, y, width, height;
  int id;
  bool visible;
  bool active;
  bool enabled;

private:
  void animateButtonClick();
};

#endif
