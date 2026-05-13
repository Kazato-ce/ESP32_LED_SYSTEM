#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>
#include <OneButton.h>
#include "BluetoothSerial.h"
#include "EEPROM.h"
#include "Font_Data.h"
#include "DHT.h"
#include "RTClib.h"
#include <Wire.h>
// ============== PIN CONFiG ================

#include "Config.h"

// ================= MODES =================

#include "ModeBase.h"
#include "ModeText.h"
#include "ModeClock.h"
#include "ModeSensor.h"
#include "ModeMusic.h"

// ======================================================
// OBJECTS
// ======================================================

BluetoothSerial SerialBT;

OneButton btn(BUTTON_PIN, true);

MD_Parola P(HARDWARE_TYPE,DATA_PIN,CLK_PIN,CS_PIN,MAX_DEVICES);

RTC_DS1307 rtc;

DHT dht(DHTPIN, DHTTYPE);

// ======================================================
// GLOBAL
// ======================================================

char msg[100] = "HELLO UIT";

int state = 0;

// ======================================================
// MODE OBJECTS
// ======================================================

ModeText   mText;
ModeClock  mClock;
ModeSensor mSensor;
ModeMusic  mMusic;

DoubleHeightMode* currentModePtr = nullptr;

// ======================================================
// SET MODE
// ======================================================

void setMode(int newMode)
{
    state = newMode;

    P.displayClear();

    switch (state) {
        // ================= TEXT =================

        case 1:

            mText.refreshData();

            mText.setupMode(PA_SCROLL_LEFT,40);

            currentModePtr = &mText;

            Serial.println("MODE TEXT");

            break;

        // ================= CLOCK =================

        case 2:

            mClock.refreshData();

            mClock.setupMode(PA_PRINT,0);

            currentModePtr = &mClock;

            Serial.println("MODE CLOCK");

            break;

        // ================= SENSOR =================

        case 3:

            mSensor.refreshData();

            mSensor.setupMode(PA_SCROLL_LEFT,50);

            currentModePtr = &mSensor;

            Serial.println("MODE SENSOR");

            break;

        // ================= MUSIC =================

        case 4:

            currentModePtr = &mMusic;

            Serial.println("MODE MUSIC");

            break;

        // ================= IDLE =================

        default:

            state = 0;

            currentModePtr = nullptr;

            Serial.println("MODE IDLE");

            break;
    }
}

// ======================================================
// BUTTON
// ======================================================

void switchMode()
{
    int next = state + 1;

    if (next > 4)
        next = 0;

    setMode(next);
}

// ======================================================
// BLUETOOTH
// ======================================================

void handleBluetooth()
{
    if (!SerialBT.available())
        return;

    String input =
        SerialBT.readStringUntil('\n');

    input.trim();

    if (input.length() == 0)
        return;

    Serial.print("BT: ");
    Serial.println(input);

    // ================= CHANGE MODE =================

    if (input.length() == 1 && isDigit(input[0]))
    {
        int target = input.toInt();

        if (target >= 0 && target <= 4)
        {
            setMode(target);
        }

        return;
    }

    // ================= CHANGE TEXT =================

    if (input.startsWith("#"))
    {
        String newText = input.substring(1);

        newText.toUpperCase();

        newText.toCharArray(msg,sizeof(msg));

        EEPROM.writeString(50,newText);

        EEPROM.commit();

        Serial.print("NEW TEXT: ");
        Serial.println(msg);

        if (state == 1)
        {
            mText.refreshData();

            mText.render();
        }
    }
}

// ======================================================
// IDLE SCREEN
// ======================================================

void runIdle()
{
    if (P.displayAnimate())
    {
        P.displayZoneText(ZONE_LOWER,"UIT",PA_CENTER,0,0,PA_PRINT,PA_NO_EFFECT);

        P.displayZoneText(ZONE_UPPER,"UIT",PA_CENTER,0,0,PA_PRINT,PA_NO_EFFECT);

        P.synchZoneStart();
    }
}

// ======================================================
// SETUP
// ======================================================

void setup()
{
    Serial.begin(115200);

    // ================= EEPROM =================

    EEPROM.begin(EEPROM_SIZE);

    // ================= BLUETOOTH =================

    SerialBT.begin("ESP32_UIT_DISPLAY");

    // ================= I2C =================

    Wire.begin(SDA_PIN,SCL_PIN);

    // ================= RTC =================

    if (!rtc.begin())
    {
        Serial.println("RTC ERROR");
    }

    // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

    // ================= DHT =================

    dht.begin();

    // ================= SOUND =================

    pinMode(SOUND_PIN, INPUT);

    // ================= MATRIX =================

    P.begin(MAX_ZONES);

    P.setZone(ZONE_LOWER,0,ZONE_SIZE - 1);

    P.setFont(ZONE_LOWER,BigFontLower);

    P.setZone(ZONE_UPPER,ZONE_SIZE,MAX_DEVICES - 1);

    P.setFont(ZONE_UPPER,BigFontUpper);

    P.setIntensity(3);

    P.setCharSpacing(P.getCharSpacing() * 2);

    // ================= BUTTON =================

    btn.attachClick(switchMode);

    // ================= EEPROM =================

    String savedMsg = EEPROM.readString(50);

    if (savedMsg.length() > 0 && savedMsg.length() < 100)
    {
        savedMsg.toCharArray(msg,sizeof(msg));
    }

    Serial.println("SYSTEM READY");
}

// ======================================================
// LOOP
// ======================================================

void loop()
{
    btn.tick();

    handleBluetooth();

    // ================= IDLE =================

    if (state == 0)
    {
        runIdle();
    }
    else
    {
        if (currentModePtr != nullptr)
        {
            currentModePtr->run();
        }
    }
}