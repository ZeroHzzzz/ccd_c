#include "image.h"
#include "cir_queue.h"
#include "init.h"
#include "binary.h"

static circlular_queue_t *ccd_image = NULL;      // 图像队列，用于存储采集到的图像数据
static circlular_queue_t *ccd_distortion = NULL; // 畸变参数队列，用于还原图像
vuint8 ccd_data[128] = {0};
vuint8 image_full_flag = 0; // 图像队列满标志，因为他是一个窗口，因此需要知道什么时候满了

vuint8 origin_map[IMAGE_ORIGIN_H][IMAGE_ORIGIN_W] = {COLOR_BLACK}; // 初始化为黑色背景

void image_init()
{
    ccd_image = circlular_queue_init(CCD_OUTLOOK, 128 * sizeof(vuint8));   // 每行128个像素，每个像素占4字节（int）
    ccd_distortion = circlular_queue_init(CCD_OUTLOOK, 2 * sizeof(float)); // 注意这里是float，记录xy方向的畸变参数（注意是差分，需要从开始的时候累加进行还原）
}

// 先记录一下图像，然后1个像素表示1cm。行与行之间关系先不管，由畸变系数决定最终图像高度，然后最后再压缩吧
// x朝下，y朝右
// 速度单位是cm/ms，采样时间30ms
void image_update(vuint16 *tsl1401_data)
{
    float distortion_params[2];

    binary_ccd_simple(tsl1401_data[0], ccd_data, 128);
    // x和y方向的畸变参数
    distortion_params[0] = encoder_get_data(&encoder_left) / ENCODER_TO_CM;             // x方向畸变参数
    distortion_params[1] = imu_get_data().gyro.z * TURN_RADIO / 1000 * CCD_SAMPLE_TIME; // y方向畸变参数，角速度转线速度，单位cm/ms

    circlular_queue_update_windows(ccd_distortion, distortion_params);
    vuint8 status = circlular_queue_update_windows(ccd_image, ccd_data);
    if (status == 1)
        image_full_flag = 1; // 队列满了
}

// 导出图像并处理图像
void image_handler()
{
    if (!image_full_flag)
        return;

    // 导出图像和畸变参数

    void **tmp_image = circlular_queue_export_reverse(ccd_image);
    void **tmp_distortion = circlular_queue_export_reverse(ccd_distortion);

    vuint8 default_offset = 30; // x方向上的偏移：(188-128)/2

    float accumulated_row_offset = 0; // 行方向累积偏移量
    float accumulated_col_offset = 0; // 列方向累积偏移量

    for (vuint8 i = 0; i < CCD_OUTLOOK; i++)
    {
        accumulated_row_offset += ((float *)tmp_distortion[i])[0]; // 计算行方向累积偏移
        accumulated_col_offset += ((float *)tmp_distortion[i])[1]; // 计算列方向累积偏移

        // 将物理单位的偏移转换成像素单位
        int row_pixel_offset = (int)(accumulated_row_offset * 2); // origin_map行方向2像素/cm
        int col_pixel_offset = (int)accumulated_col_offset;       // origin_map列方向1像素/cm

        vuint8 real_col_offset = default_offset - col_pixel_offset; // 更新当前行的列绝对偏移量

        // 填充：需检查填充逻辑
        // if (row_pixel_offset > 0 && i - row_pixel_offset >= 0)
        // {
        //     memcpy(&origin_map[i - row_pixel_offset][0], tmp_image[i], 128);
        // }
        if (col_pixel_offset >= 2) // 如果大于两行说明中间是不需要插的
        {
            for (vuint8 col = 1; col < col_pixel_offset; col++)
            {
                memcpy(&origin_map[i][real_col_offset], tmp_image[i], 128);
            }
        }

        if (real_col_offset < 0) // 也就是超出左边界了，那就需要截断图像部分
        {
            // 计算实际可复制的数据起始位置和长度
            int start_pos = -real_col_offset;
            int copy_length = 128 - start_pos;

            if (copy_length > 0) // 部分数据仍在范围内
            {
                memcpy(&origin_map[i][0], &((vuint8 *)tmp_image[i])[start_pos], copy_length);
            }
        }
        else if (real_col_offset > (IMAGE_ORIGIN_W - 128)) // 超出右边界了
        {
            // 计算实际可复制的数据长度
            int copy_length = IMAGE_ORIGIN_W - real_col_offset;

            if (copy_length > 0) // 部分数据仍在范围内
            {
                memcpy(&origin_map[i][real_col_offset], tmp_image[i], copy_length);
            }
        }
        else
        {

            memcpy(&origin_map[i][real_col_offset], tmp_image[i], 128);
        }
    }

    // 释放内存
    circlular_queue_free_exported(tmp_image, CCD_OUTLOOK);
    circlular_queue_free_exported(tmp_distortion, CCD_OUTLOOK);
}