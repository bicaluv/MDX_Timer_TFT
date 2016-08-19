#ifndef Debug_h
#define Debug_h

#define DEBUG // comment this line if debug output is to be disabled


#ifdef DEBUG
#include <HardwareSerial.h>
#define DEBUG_PRINTLN(message)  {Serial.println(message);Serial.flush();}
#define DEBUG_PRINT(message)  {Serial.print(message);Serial.flush();}
#else
#define DEBUG_PRINTLN(message)
#define DEBUG_PRINT(message)
#endif

#endif
