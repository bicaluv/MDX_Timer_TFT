#include "Motor.h"


Motor::Motor() {
  pinMode(MOTOR_PIN, OUTPUT);
  digitalWrite(MOTOR_PIN, LOW);
}

void Motor::start() {
  digitalWrite(MOTOR_PIN, HIGH);
}

void Motor::stop() {
  digitalWrite(MOTOR_PIN, LOW);
}

