#ifndef MODESENSOR_H
#define MODESENSOR_H

#include "Globals.h"

#include "ModeBase.h"

class ModeSensor : public DoubleHeightMode{
public:
    void refreshData() override
    {
        float t = dht.readTemperature();
        float h = dht.readHumidity();

        if (isnan(t) || isnan(h))
        {
            strcpy(_buffer, "DHT ERROR");
            return;
        }

        sprintf(_buffer,"%.0fC %.0f%%",t,h);
    }
};

#endif