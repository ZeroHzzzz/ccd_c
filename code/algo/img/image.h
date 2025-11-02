#ifndef ALGO_IMG_IMAGE_H
#define ALGO_IMG_IMAGE_H

#include "zf_common_typedef.h"

#define IMAGE_HEIGHT 60
#define IMAGE_SAMPLE_TIME 30 // ms

typedef enum
{
    COLOR_BLACK = 0,
    COLOR_WHITE = 255,
} image_color_e;

void image_init();
void image_update(vuint16 *tsl1401_data);

#endif