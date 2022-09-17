#ifndef Motor_h
#define Motor_h

#include <Arduino.h>
#include "Debug.h"


#define MOTOR_PIN     19

class Motor {
public:
  Motor();

  void start();
  void stop();
};

#endif
