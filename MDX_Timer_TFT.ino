/*
 * Matthias Mueller
 * copyright (c) 2016
 * Please leave this note within the code.
 */
#include <Arduino.h>

#include "Debug.h"

#include "Motor.h"
#include "UI.h"
#include "Settings.h"


Motor motor;
Settings settings;
UI ui;


int currentState = MANUAL_BUTTON;
unsigned int currentTimerValue = 0;
unsigned long targetTime = 0;

// logic
bool prog = false;
bool start = false;
bool pause = false;


// *** Begin Callback functions ***

void onManualClick();
void onTimer1Click();
void onTimer2Click();

void onUpClick();
void onDownClick();

void onPlayClick();
void onPauseClick();

void onRecClick();

// *** End Callback functions ***


void setup() {
#ifdef DEBUG
  Serial.begin(115200);
#endif
  
  ui.init();
  
  ui.showSplashScreen(settings.getName(), settings.getVersion());

  ui.addButton(MANUAL_BUTTON, onManualClick);
  ui.addButton(TIMER1_BUTTON, onTimer1Click);
  ui.addButton(TIMER2_BUTTON, onTimer2Click);

  ui.addButton(UP_BUTTON, onUpClick);
  ui.addButton(DOWN_BUTTON, onDownClick);

  ui.addButton(PLAY_BUTTON, onPlayClick);
  ui.addButton(PAUSE_BUTTON, onPauseClick);
  ui.addButton(REC_BUTTON, onRecClick);
  ui.addButton(REC_BUTTON_ACTIVE, onRecActiveClick);

  currentState = settings.getStartState();
  if (currentState == TIMER1_BUTTON || currentState == TIMER2_BUTTON) {
    currentTimerValue = settings.getTimers()[currentState - 1];
  }

  delay(2000);
  
  ui.refresh();
    
  ui.activateButton(PLAY_BUTTON);
  ui.activateButton(REC_BUTTON);

  ui.activateButton(currentState);

  ui.updateContent(currentTimerValue);
}


void loop() {

  if (start && (millis() > targetTime && currentState != MANUAL_BUTTON)) {
    motor.stop();
    start = false;
    pause = false;
    
    currentTimerValue = settings.getTimers()[currentState - 1];
    ui.activateButton(PLAY_BUTTON);
    ui.updateContent(currentTimerValue);
  }
  if (start) {
    if (currentState == MANUAL_BUTTON) {
      currentTimerValue = millis() - targetTime;
    }
    else {
      currentTimerValue = targetTime - millis();
    }
    currentTimerValue /= 100;
    ui.updateContent(currentTimerValue);
  }  

  ui.checkButtonPressed();
}


// *********************** CLICK LOGIC ****************************************

void onPlayClick() {
  if (start) {
    return;
  }
  
  DEBUG_PRINTLN("Play pressed");

  DEBUG_PRINT("start with time=");
  DEBUG_PRINTLN(currentTimerValue);

  ui.activateButton(PAUSE_BUTTON);
  delay(200);

  start = true;
  pause = false;
  prog = false;
  ui.activateButton(REC_BUTTON);
  
  motor.start();
  
  targetTime = millis();
  if (currentState == MANUAL_BUTTON) {
    if (pause) {
      // continue
      targetTime -= currentTimerValue * 100;
    }
  }
  else {
    targetTime += currentTimerValue * 100;
  }
}

void onPauseClick() {
  if (!start) {
    return;
  }
  
  DEBUG_PRINTLN("Pause pressed");

  DEBUG_PRINT("pause with time=");
  DEBUG_PRINTLN(currentTimerValue);

  motor.stop();
      
  start = false;
  pause = true;
  prog = false;
  ui.activateButton(REC_BUTTON);

  ui.activateButton(PLAY_BUTTON);

  delay(300);
}

void onManualClick() {
  if (start) {
    return;
  }

  if (currentState == MANUAL_BUTTON) {
    return;
  }
  
  DEBUG_PRINTLN("Manual pressed");
  
  currentState = MANUAL_BUTTON;
  ui.activateButton(currentState);
  settings.setStartState(currentState);

  currentTimerValue = 0;
  ui.updateContent(currentTimerValue);

  prog = false;
  ui.activateButton(REC_BUTTON);
}

void onTimer1Click() {
  if (start) {
    return;
  }
  
  DEBUG_PRINTLN("Timer1 pressed");

  if (prog) {
    settings.getTimers()[0] = currentTimerValue;
    settings.save();
    prog = false;
    ui.activateButton(REC_BUTTON);
  }
  
  if (currentState == TIMER1_BUTTON) {
    return;
  }

  currentTimerValue = settings.getTimers()[0];
  ui.updateContent(currentTimerValue);

  currentState = TIMER1_BUTTON;
  ui.activateButton(currentState);
  settings.setStartState(currentState);
}

void onTimer2Click() {
  if (start) {
    return;
  }
  
  DEBUG_PRINTLN("Timer2 pressed");
  
  if (prog) {
    settings.getTimers()[1] = currentTimerValue;
    settings.save();
    prog = false;
    ui.activateButton(REC_BUTTON);
  }
  
  if (currentState == TIMER2_BUTTON) {
    return;
  }
  
  currentTimerValue = settings.getTimers()[1];
  ui.updateContent(currentTimerValue);
  
  currentState = TIMER2_BUTTON;
  ui.activateButton(currentState);
  settings.setStartState(currentState);
}


void onUpClick() {
  if (start || currentState == MANUAL_BUTTON) {
    return;
  }
  
  DEBUG_PRINTLN("Up pressed");

  currentTimerValue++;
  ui.updateContent(currentTimerValue);

  prog = false;
  ui.activateButton(REC_BUTTON);
}

void onDownClick() {
  if (start || currentState == MANUAL_BUTTON) {
    return;
  }
  
  DEBUG_PRINTLN("Down pressed");

  currentTimerValue--;
  ui.updateContent(currentTimerValue);

  prog = false;
  ui.activateButton(REC_BUTTON);
}


void onRecClick() {
  if (start) {
    return;
  }
  
  DEBUG_PRINTLN("Rec pressed");

  prog = true;
  ui.activateButton(REC_BUTTON_ACTIVE);

  delay(200);
}

void onRecActiveClick() {
  if (start) {
    return;
  }
  
  DEBUG_PRINTLN("Rec Active pressed");

  prog = false;
  ui.activateButton(REC_BUTTON);

  delay(200);
}
