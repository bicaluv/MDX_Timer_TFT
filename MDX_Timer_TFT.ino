
#include <Arduino.h>

#include "Debug.h"

#include "Motor.h"
#include "UI.h"
#include "Settings.h"
#include "StateMachine.h"


Motor motor;
Settings settings;
UI ui;
StateMachine stateMachine;

int currentState = INITIAL_STATE;


// *** Begin WireUp functions ***

int timer0Handle(int evt);
void timer0Update(int newValue);

int timer1Handle(int evt);
void timer1Update(int newValue);

int timer2Handle(int evt);
void timer2Update(int newValue);

int playPauseHandle(int evt);
void playPauseUpdate(int newValue);

int recHandle(int evt);
void recUpdate(int newValue);

int upHandle(int evt);
void upUpdate(int newValue);

int downHandle(int evt);
void downUpdate(int newValue);

// *** End WireUp functions ***


void setup() {
#ifdef DEBUG
  Serial.begin(115200);
#endif
  
  ui.init(settings.getTimers());
  
  ui.showSplashScreen(settings.getName(), settings.getVersion());

  currentState = settings.getStartState();
  stateMachine.init(currentState);
  stateMachine.addState(TIMER0_STATE, timer0Handle, timer0Update);
  stateMachine.addState(TIMER1_STATE, timer1Handle, timer1Update);
  stateMachine.addState(TIMER2_STATE, timer2Handle, timer2Update);
  stateMachine.addState(PLAY_PAUSE_STATE, playPauseHandle, playPauseUpdate);
  stateMachine.addState(REC_STATE, recHandle, recUpdate);
  stateMachine.addState(UP_STATE, upHandle, upUpdate);
  stateMachine.addState(DOWN_STATE, downHandle, downUpdate);
}

void loop() {
  
  //motor.start(settings.getTimers()[0] * 1000);

  int newValue = ui.update();
  
  if (newValue != currentState) {
    currentState = stateMachine.update(newValue);
  }
}

// ***************************************************************


int timer0Handle(int evt) {
  switch(evt) {
    case 0:
    return 1;
    break;
    case 1:
    return 2;
    break;
    default:
    return 0;
    break;
  }
}

void timer0Update(int newValue) {
  ui.refresh(newValue);

  settings.setStartState(newValue);
}

int timer1Handle(int evt) {
  switch(evt) {
    case 0:
    return 1;
    break;
    case 1:
    return 2;
    break;
    default:
    return 0;
    break;
  }
}

void timer1Update(int newValue) {
  ui.refresh(newValue);

  settings.setStartState(newValue);
}

int timer2Handle(int evt) {
  switch(evt) {
    case 0:
    return 1;
    break;
    case 1:
    return 2;
    break;
    default:
    return 0;
    break;
  }
}

void timer2Update(int newValue) {
  ui.refresh(newValue);

  settings.setStartState(newValue);
}

int playPauseHandle(int evt) {
  switch(evt) {
    case 0:
    return 1;
    break;
    case 1:
    return 2;
    break;
    default:
    return 0;
    break;
  }
}

void playPauseUpdate(int newValue) {
  ui.refresh(newValue);

  settings.setStartState(newValue);
}

int recHandle(int evt) {
  switch(evt) {
    case 0:
    return 1;
    break;
    case 1:
    return 2;
    break;
    default:
    return 0;
    break;
  }
}

void recUpdate(int newValue) {
  ui.refresh(newValue);

  settings.setStartState(newValue);
}

int upHandle(int evt) {
  switch(evt) {
    case 0:
    return 1;
    break;
    case 1:
    return 2;
    break;
    default:
    return 0;
    break;
  }
}

void upUpdate(int newValue) {
  ui.refresh(newValue);

  settings.setStartState(newValue);
}

int downHandle(int evt) {
  switch(evt) {
    case 0:
    return 1;
    break;
    case 1:
    return 2;
    break;
    default:
    return 0;
    break;
  }
}

void downUpdate(int newValue) {
  ui.refresh(newValue);

  settings.setStartState(newValue);
}

