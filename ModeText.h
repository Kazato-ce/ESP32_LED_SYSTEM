#ifndef MODETEXT_H
#define MODETEXT_H

#include "Globals.h"

#include "ModeBase.h"

class ModeText : public DoubleHeightMode
{
public:
    void refreshData() override
    {
        strcpy(_buffer, msg);
    }
};

#endif