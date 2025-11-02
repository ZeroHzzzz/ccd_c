#ifndef _SYSTEM_INIT_H_
#define _SYSTEM_INIT_H_

#include "zf_common_headfile.h"
#include "encoder.h"
#include "motor.h"
#include "imu.h"
#include "attitude.h"

void device_init(void);

extern encoder_obj_t encoder_left, encoder_right;

#endif