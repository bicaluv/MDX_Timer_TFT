#include "Button.h"
#include "UI.h"


#define BUTTON_RADIUS  3


Button::Button() {
}

void Button::drawButton() {
  int x1, y1, w, h, hw, hh;
  x1 = x;
  y1 = y;
  w = width;
  h = height;
  hw = w / 2;
  hh = h / 2;

  if (!visible) {
    gfx->fillRect(x1 + 1, y1 + 1, w - 2, h - 2, BLACK); // clear previous drawing
    return;
  }

  // draw frame
  int frameColor = WHITE;
  if (active) {
    frameColor = GREEN;
  }
  if (id != UP_BUTTON && id != DOWN_BUTTON) {
    gfx->drawRoundRect(x1, y1, w, h, BUTTON_RADIUS, frameColor);
  }

  if (id == REC_BUTTON) {
    gfx->fillCircle(x1 + hw, y1 + hh, 12, RED);
    return;
  }
  else if (id == REC_BUTTON_ACTIVE) {
    gfx->fillCircle(x1 + hw, y1 + hh, 12, YELLOW);
    return;
  }
  else if (id == PAUSE_BUTTON) {
    gfx->fillRect(x1 + hw - 12, y1 + 7, 10, h - 16, GREEN);
    gfx->fillRect(x1 + hw + 2, y1 + 7, 10, h - 16, GREEN);
    return;
  }
  else if (id == PLAY_BUTTON) {
    gfx->fillTriangle(x1 + 30, y1 + 7, x1 + 30, y1 + h - 7, x1 + w - 30, y1 + hh, GREEN);
    return;
  }
  else if (id == DOWN_BUTTON) {
    gfx->setCursor(x1, gfx->height() / 2 + 5);
    gfx->print("-");
    return;
  }
  else if (id == UP_BUTTON) {
    gfx->setCursor(gfx->width() - 20, gfx->height() / 2 + 5);
    gfx->print("+");
    return;
  }
  
  String label;
  if (id == MANUAL_BUTTON) {
    label = "m";
  }
  else if (id == TIMER1_BUTTON) {
    label = "1";
  }
  else if (id == TIMER2_BUTTON) {
    label = "2";
  }

  gfx->setTextColor(frameColor);
  gfx->setCursor(x1 + 35, y1 + 35);
  gfx->print(label);
  gfx->setTextColor(WHITE);
  
  if (!active) {
    frameColor = BLACK;
  }
  // emphasise frame
  gfx->drawRoundRect(x1 + 1, y1 + 1, w - 2, h - 2, BUTTON_RADIUS, frameColor);
  gfx->drawRoundRect(x1 + 2, y1 + 2, w - 4, h - 4, BUTTON_RADIUS, frameColor);
}

void Button::animateButtonClick() {
  if (!visible) {
    return;
  }
  
  int x1, y1, w, h;
  // preserve button frame
  x1 = x + 1;
  y1 = y + 1;
  w = width - 2;
  h = height - 2;
  
  gfx->fillRect(x1, y1, w, h, GRAY);
  delay(150);
  gfx->fillRect(x1, y1, w, h, BLACK);
  drawButton();  
}

void Button::showButtonAndDraw(bool isVisible) {
  visible = isVisible;
  drawButton();  
}

bool Button::hit(uint16_t xpos, uint16_t ypos) {
  bool hitTest= visible && enabled && (xpos > x) && (xpos < (x + width)) && (ypos > y) && (ypos < (y + height));
  if (hitTest) {
    animateButtonClick();
    callback();
  }

  return hitTest;
}

void Button::activateButtonAndDraw(bool activate) {
  active = activate;
  drawButton();  
}
