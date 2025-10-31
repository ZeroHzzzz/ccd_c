#include "ccd_handler.h"

void ccd_handler(vuint16 *tsl1401, vuint16 *output, vuint16 length)
{
    vuint16 minn = 65535;
    vuint16 maxn = 0;
    // tft180_show_wave(0, 0, tsl1401, 128, 256, 160, 127);
    for (vuint16 i = 0; i < length; i++)
    {
        if (tsl1401[i] < minn)
        {
            minn = tsl1401[i];
        }
        if (tsl1401[i] > maxn)
        {
            maxn = tsl1401[i];
        }
    }

    vuint16 threshold = (maxn + minn) / 2;

    for (size_t i = 0; i < length; i++)
    {
        if (tsl1401[i] > threshold)
        {
            output[i] = 100; // 高于阈值，设置为高值
        }
        else
        {
            output[i] = 50; // 低于或等于阈值，设置为低值
        }
    }
}