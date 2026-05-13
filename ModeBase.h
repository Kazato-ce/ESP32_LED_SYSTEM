#ifndef MODEBASE_H
#define MODEBASE_H

#include "Globals.h"

class DoubleHeightMode
{
protected:
    char _buffer[120];

    textEffect_t _effect;
    uint16_t _speed;

public:

    // ================= PURE VIRTUAL =================

    virtual void refreshData() = 0;

    // ================= VIRTUAL =================

    virtual void setupMode(textEffect_t effect, uint16_t speed)
    {
        _effect = effect;
        _speed = speed;

        refreshData();

        render();
    }

    virtual void render()
    {
        P.displayClear();

        P.displayZoneText(ZONE_LOWER,_buffer,PA_CENTER,_speed,0,_effect,_effect);

        P.displayZoneText(ZONE_UPPER,_buffer,PA_CENTER,_speed,0,_effect,_effect);

        P.synchZoneStart();
    }

    virtual void run()
    {
        if (P.displayAnimate())
        {
            refreshData();

            render();
        }
    }

    // ================= DESTRUCTOR =================

    virtual ~DoubleHeightMode() {}
};

#endif