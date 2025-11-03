#ifndef ALGO_IMG_IMAGE_H
#define ALGO_IMG_IMAGE_H

#include "zf_common_typedef.h"

#define CCD_OUTLOOK 45     // 前瞻，单位为cm
#define CCD_SAMPLE_TIME 30 // ccd曝光时间，单位为ms

#define ENCODER_TO_CM 130 // 表示130个编码器脉冲为1cm

#define IMAGE_ORIGIN_W 188
#define IMAGE_ORIGIN_H 60

typedef enum
{
    COLOR_BLACK = 0,
    COLOR_WHITE = 255,
} image_color_e;

void image_init();
void image_update(vuint16 *tsl1401_data);

#endif