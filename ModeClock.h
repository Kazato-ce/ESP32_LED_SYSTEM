#ifndef MODECLOCK_H
#define MODECLOCK_H

#include "Globals.h"

#include "ModeBase.h"

class ModeClock : public DoubleHeightMode
{
public:
    void refreshData() override
    {
        DateTime now = rtc.now();

        sprintf(_buffer,"%02d:%02d:%02d",now.hour(),now.minute(),now.second());
    }
};

#endif