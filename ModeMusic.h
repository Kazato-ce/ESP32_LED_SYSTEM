#ifndef MODEMUSIC_H
#define MODEMUSIC_H

#include "ModeBase.h"
#include "Globals.h"

class ModeMusic : public DoubleHeightMode
{
private:
    int lastLevels[64]; // Lưu mức độ của từng cột để làm mượt riêng lẻ
    unsigned long lastUpdate = 0;

public:
    void setupMode(textEffect_t effect, uint16_t speed) override {
        P.displayClear();
        for(int i=0; i<64; i++) lastLevels[i] = 0;
    }

    void refreshData() override
    {
        MD_MAX72XX* mx = P.getGraphicObject();
        int raw = analogRead(SOUND_PIN);

        // Giả lập sóng nhạc nhảy ngẫu nhiên quanh giá trị Analog để các cột nhìn khác nhau
        for (int x = 0; x < 64; x++)
        {
            // Chuyển giá trị analog thành độ cao (0-16)
            // Lưu ý: giá trị 1500-3500 tùy thuộc vào cảm biến của bạn, có thể cần chỉnh lại
            int targetLevel = map(raw, 1800, 3800, 0, 16); 
            
            // Thêm chút biến động cho các cột khác nhau nhìn cho "vui mắt"
            targetLevel += random(-2, 3); 
            targetLevel = constrain(targetLevel, 0, 16);

            // Làm mượt (Smoothing) cho từng cột
            lastLevels[x] = (lastLevels[x] * 1 + targetLevel) / 2;

            // Xóa cột hiện tại trước khi vẽ (để không bị nháy cả màn hình)
            for (int y = 0; y < 16; y++) {
                mx->setPoint(y, x, false);
            }

            // Vẽ cột mới
            for (int y = 0; y < lastLevels[x]; y++)
            {
                mx->setPoint(15 - y, x, true);
            }
        }
    }

    void run() override
    {
        if (millis() - lastUpdate >= 30) 
        {
            lastUpdate = millis();
            refreshData();
            // Chỉ dùng update() của tầng thấp, không dùng P.displayAnimate vì đây là vẽ pixel trực tiếp
            P.getGraphicObject()->update(); 
        }
    }
};

#endif