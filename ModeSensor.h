#ifndef MODESENSOR_H
#define MODESENSOR_H

#include "Globals.h"
#include "ModeBase.h"

class ModeSensor : public DoubleHeightMode {
private:
    char _bufUpper[10]; // Buffer riêng cho hàng trên (Nhiệt độ)
    char _bufLower[10]; // Buffer riêng cho hàng dưới (Độ ẩm)

public:
    void refreshData() override
    {
        float t = dht.readTemperature();
        float h = dht.readHumidity();

        if (isnan(t) || isnan(h))
        {
            strcpy(_bufUpper, "TEMP");
            strcpy(_bufLower, "ERR");
            return;
        }

        // Định dạng dữ liệu cho từng hàng
        sprintf(_bufUpper, "%.0fC", t);
        sprintf(_bufLower, "%.0f%%", h);
    }

    void setupMode(textEffect_t effect, uint16_t speed) override
    {
        refreshData();
        P.displayClear();

        // Thiết lập hiển thị tĩnh (PA_PRINT) và căn giữa (PA_CENTER)
        // Zone trên hiển thị Nhiệt độ
        P.displayZoneText(ZONE_UPPER, _bufUpper, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
        
        // Zone dưới hiển thị Độ ẩm
        P.displayZoneText(ZONE_LOWER, _bufLower, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);

        P.synchZoneStart();
    }

    void run() override
    {
        // Cập nhật dữ liệu mới sau một khoảng thời gian (ví dụ mỗi 2 giây)
        static uint32_t lastUpdate = 0;
        if (millis() - lastUpdate >= 2000)
    {
    lastUpdate = millis();
    refreshData();
    
    P.displayZoneText(ZONE_UPPER, _bufUpper, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);
    
    
    P.displayZoneText(ZONE_LOWER, _bufLower, PA_CENTER, 0, 0, PA_PRINT, PA_NO_EFFECT);

    // Reset các zone để hiển thị giá trị mới
    P.displayReset(ZONE_UPPER);
    P.displayReset(ZONE_LOWER);
    }

        P.displayAnimate();
    }
};

#endif