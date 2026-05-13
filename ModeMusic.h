#ifndef MODEMUSIC_H
#define MODEMUSIC_H

#include "ModeBase.h"
#include "Globals.h"

class ModeMusic : public DoubleHeightMode
{
private:

    int lastLevel = 0;

public:

    void refreshData() override
    {
        int raw = analogRead(SOUND_PIN);

        int level =
            map(raw, 1500, 3500, 0, 16);

        if (level < 0) level = 0;
        if (level > 16) level = 16;

        // smooth
        level =
            (lastLevel * 2 + level) / 3;

        lastLevel = level;

        // clear matrix
        P.displayClear();

        MD_MAX72XX* mx =
            P.getGraphicObject();

        // draw bars
        for (int x = 0; x < 16; x++)
        {
            for (int y = 0; y < level; y++)
            {
                mx->setPoint(
                    15 - y,
                    x,
                    true
                );
            }
        }

        P.displayReset();
    }

    void run()
    {
        refreshData();
    }
};

#endif