#ifndef CONFIG_H
#define CONFIG_H

#include <MD_Parola.h>
#include <MD_MAX72xx.h>

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

#define MAX_ZONES     2
#define ZONE_SIZE     8
#define MAX_DEVICES   (MAX_ZONES * ZONE_SIZE)

#define ZONE_LOWER    0
#define ZONE_UPPER    1

// ================= MATRIX =================

#define DATA_PIN      23
#define CLK_PIN       18
#define CS_PIN        5

// ================= BUTTON =================

#define BUTTON_PIN    27

// ================= RTC ====================

#define SDA_PIN       21
#define SCL_PIN       22

// ================= DHT ====================

#define DHTPIN        32
#define DHTTYPE       DHT11
//================= SOUND =================

#define SOUND_PIN 34

// ================= EEPROM =================

#define EEPROM_SIZE   512

#endif