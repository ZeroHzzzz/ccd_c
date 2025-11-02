#ifndef _DEVICE_ENCODER_H_
#define _DEVICE_ENCODER_H_

#include "zf_common_typedef.h"
#include "zf_driver_encoder.h"

#define ENCODER_TO_CM 130 // 表示130个编码器单位为1cm

typedef struct
{
    encoder_index_enum index;
} encoder_obj_t;

encoder_obj_t encoder_init(encoder_index_enum index, encoder_channel1_enum pin_a, encoder_channel2_enum pin_b);
vint16 encoder_get_data(encoder_obj_t *obj);
void encoder_reset(encoder_obj_t *obj);

#endif