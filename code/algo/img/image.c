#include "image.h"
#include "cir_queue.h"

static circlular_queue_t *ccd_image = NULL;      // 图像队列，用于存储采集到的图像数据
static circlular_queue_t *ccd_distortion = NULL; // 畸变参数队列，用于还原图像

void image_init()
{
    ccd_image = circlular_queue_init(IMAGE_HEIGHT, 128 * sizeof(vuint16));   // 每行128个像素，每个像素占4字节（int）
    ccd_distortion = circlular_queue_init(IMAGE_HEIGHT, 2 * sizeof(vint16)); // 注意这里是int，记录xy方向的畸变参数（注意是差分，需要从开始的时候累加进行还原）
}

void image_update(vuint16 *image_line, vint16 *distortion_params)
{
    circlular_queue_enqueue(ccd_image, (uint8_t *)image_line);
    circlular_queue_enqueue(ccd_distortion, (uint8_t *)distortion_params);
}