#ifndef GLOBALS_H
#define GLOBALS_H

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include "BluetoothSerial.h"
#include "DHT.h"
#include "RTClib.h"

// ======================================================
// EXTERN OBJECTS
// ======================================================

extern MD_Parola P;

extern BluetoothSerial SerialBT;

extern RTC_DS1307 rtc;

extern DHT dht;

// ======================================================
// EXTERN VARIABLES
// ======================================================

extern char msg[100];

extern int state;

#endif