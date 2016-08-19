#ifndef UI_h
#define UI_h

#include <Arduino.h>
#include "Debug.h"
#include "StateMachine.h"


#define RUN_INFINITE  0

//typedef void (*callback)();

typedef struct {
  unsigned int      pos_x, pos_y, width, height;
  String label;
  void (*callback)();
} ButtonType;


class UI {
public:
  UI();

  void init(int *timers);
  
  void showSplashScreen(String name, String version);
  int update();
  void refresh(int newValue);
  
private:
  void addButton(ButtonType newBtn);
  
  ButtonType _buttons[NUM_STATES];

};

#endif
