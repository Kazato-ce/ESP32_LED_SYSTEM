#ifndef MODECLOCK_H
#define MODECLOCK_H

#include "Globals.h"
#include "ModeBase.h"

class ModeClock : public DoubleHeightMode
{
private:
    unsigned long lastClock = 0;
    bool blinkColon = true;

public:
    void refreshData() override
    {
        DateTime now = rtc.now();

        // Nháy dấu :
        if (blinkColon)
            sprintf(_buffer, "%02d:%02d", now.hour(), now.minute());
        else
            sprintf(_buffer, "%02d %02d", now.hour(), now.minute());
    }

    void setupMode(textEffect_t effect, uint16_t speed) override
    {
        refreshData();
        P.displayClear();

        P.displayZoneText(ZONE_LOWER, _buffer, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
        P.displayZoneText(ZONE_UPPER, _buffer, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);

        P.synchZoneStart();
    }

    void run() override
    {
        // đổi trạng thái nháy mỗi 500ms
        if (millis() - lastClock >= 500)
        {
            lastClock = millis();

            blinkColon = !blinkColon;
            refreshData();

            P.displayZoneText(ZONE_LOWER, _buffer, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
            P.displayZoneText(ZONE_UPPER, _buffer, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);

            P.displayReset(ZONE_LOWER);
            P.displayReset(ZONE_UPPER);
        }

        P.displayAnimate();
    }
};

#endif