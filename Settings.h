#ifndef Settings_h
#define Settings_h

#include <Arduino.h>
#include <String.h>
#include "Debug.h"


#define MAX_NUM_TIMERS  2


struct EEPROMSettings {
  String version;
  int startState;
  int values[MAX_NUM_TIMERS];
};



class Settings {
public:
  Settings();
  
  void reset();
  void save();
  
  String getName();
  String getVersion();
  int* getTimers();
  int getStartState();
  void setStartState(int startState);
  
private:
  EEPROMSettings _currentSettings;
};

#endif
