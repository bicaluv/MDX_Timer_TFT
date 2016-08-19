
#include "Settings.h"

#include <EEPROM.h>


#define CURRENT_VERSION   "0.1"
#define MACHINE_NAME      "MDX"




Settings::Settings() {
  // read from EEPROM
  EEPROM.get(0, _currentSettings);

  if (_currentSettings.version != CURRENT_VERSION) {
    reset();
  }
}

void Settings::reset() {
    // read from EEPROM
  _currentSettings.version = CURRENT_VERSION;
  _currentSettings.startState = 0;
  _currentSettings.values[0] = 50;
  _currentSettings.values[1] = 100;

  save();
}


void Settings::save() {
  EEPROM.put(0, _currentSettings);
}


int Settings::getStartState() {
  return _currentSettings.startState;
}

void Settings::setStartState(int startState) {
  _currentSettings.startState = startState;
}

String Settings::getName() {
  return MACHINE_NAME;
}

String Settings::getVersion() {
  return CURRENT_VERSION;
}

int* Settings::getTimers() {
  return _currentSettings.values;
}


