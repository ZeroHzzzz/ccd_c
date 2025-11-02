#include "binary.h"
#include "image.h"

void binary_ccd_simple(vuint16 *input, vuint8 *output, vuint16 length)
{
    vuint16 min_val = 65535;
    vuint16 max_val = 0;

    for (vuint16 i = 0; i < length; i++)
    {
        if (input[i] < min_val)
        {
            min_val = input[i];
        }
        if (input[i] > max_val)
        {
            max_val = input[i];
        }
    }

    vuint16 threshold = (max_val + min_val) / 2;

    for (vuint16 i = 0; i < length; i++)
    {
        if (input[i] > threshold)
        {
            output[i] = COLOR_WHITE; // Above threshold
        }
        else
        {
            output[i] = COLOR_BLACK; // Below or equal to threshold
        }
    }
}