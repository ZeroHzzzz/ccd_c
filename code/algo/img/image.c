#include "image.h"
#include "cir_queue.h"
#include "init.h"
#include "binary.h"

static circlular_queue_t *ccd_image = NULL;      // 图像队列，用于存储采集到的图像数据
static circlular_queue_t *ccd_distortion = NULL; // 畸变参数队列，用于还原图像
vuint8 ccd_data[128] = {0};
vuint8 image_full_flag = 0; // 图像队列满标志，因为他是一个窗口，因此需要知道什么时候满了

void image_init()
{
    ccd_image = circlular_queue_init(IMAGE_HEIGHT, 128 * sizeof(vuint8));   // 每行128个像素，每个像素占4字节（int）
    ccd_distortion = circlular_queue_init(IMAGE_HEIGHT, 2 * sizeof(float)); // 注意这里是float，记录xy方向的畸变参数（注意是差分，需要从开始的时候累加进行还原）
}

// 先记录一下图像，然后1个像素表示1cm。行与行之间关系先不管，由畸变系数决定最终图像高度，然后最后再压缩吧
// x朝下，y朝右
// 速度单位是cm/ms，采样时间30ms
void image_update(vuint16 *tsl1401_data)
{
    vint16 distortion_params[2];

    binary_ccd_simple(tsl1401_data[0], ccd_data, 128);
    // x和y方向的畸变参数
    distortion_params[0] = encoder_get_data(&encoder_left) / ENCODER_TO_CM;               // x方向畸变参数
    distortion_params[1] = imu_get_data().gyro.z * TURN_RADIO / 1000 * IMAGE_SAMPLE_TIME; // y方向畸变参数，角速度转线速度，单位cm/ms

    circlular_queue_update_windows(ccd_distortion, distortion_params);
    vuint8 status = circlular_queue_update_windows(ccd_image, ccd_data);
    if (status == 1)
        image_full_flag = 1; // 队列满了
}