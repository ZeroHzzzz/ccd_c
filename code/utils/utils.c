#include "utils.h"
#include "image.h"

vint16 find_widest_line(vuint16 *array, vuint16 length)
{
    vint16 max_length = 0;
    vint16 current_length = 0;

    for (vuint16 i = 0; i < length; i++)
    {
        if (array[i] == COLOR_WHITE)
        {
            current_length++;
            if (current_length > max_length)
            {
                max_length = current_length;
            }
        }
        else
        {
            current_length = 0;
        }
    }

    return max_length;
}