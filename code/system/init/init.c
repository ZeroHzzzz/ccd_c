#include "init.h"

encoder_obj_t encoder_left, encoder_right;

void device_init(void)
{
    tft180_set_dir(TFT180_PORTAIT);
    tft180_init();

    tsl1401_init(); // TSL1401 初始化
    encoder_left = encoder_init(TIM6_ENCODER, TIM6_ENCODER_CH1_P20_3, TIM6_ENCODER_CH2_P20_0);

    imu_init(IMU_DEVICE_963RA);
    attitude_init(ATTITUDE_EKF);
    pit_ms_init(CCU60_CH0, 1);
    // pit_ms_init(CCU60_CH1, 20000);
}