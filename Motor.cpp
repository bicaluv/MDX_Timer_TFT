#include "Motor.h"


Motor::Motor() {
  pinMode(MOTOR_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, HIGH);
}

void Motor::start() {
  DEBUG_PRINTLN("start");
  
  digitalWrite(MOTOR_PIN, LOW);
}

void Motor::stop() {
  DEBUG_PRINTLN("stop");
  
  digitalWrite(MOTOR_PIN, HIGH);
}
