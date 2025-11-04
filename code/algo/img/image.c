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
    void **tmp_distortion = circlular_queue_export_reverse(ccd_distortion); // 这里畸变系数记录的是第i行相对于第i-1行的偏移

    vuint8 default_offset = 30; // x方向上的偏移：(188-128)/2

    // float 单位的偏移量。至于这里为什么用float只是为了抑制误差（真可以吗？）
    float accumulated_row_offset = 0.0f; // 累计的行偏移量，单位cm
    float accumulated_col_offset = 0.0f; // 累计的列偏移量，单位cm

    vuint8 curr_row = 0; // 当前行指针

    for (vuint8 i = 0; i < CCD_OUTLOOK && curr_row < IMAGE_ORIGIN_H; i++)
    {
        float curr_row_offset = ((float *)tmp_distortion[i])[0];
        float curr_col_offset = ((float *)tmp_distortion[i])[1];

        // 保存插值前的累计偏移
        float prev_accumulated_row = accumulated_row_offset;
        float prev_accumulated_col = accumulated_col_offset;

        // 更新累计偏移量
        accumulated_row_offset += curr_row_offset;
        accumulated_col_offset += curr_col_offset;

        // 计算插值需要的参数
        int total_pixel_rows = (int)(curr_row_offset * 2); // 总需要填充的像素行数

        if (total_pixel_rows <= 0)
        {
            total_pixel_rows = 1; // 至少填充一行
        }

        // 为当前行数据创建插值行
        for (int insert_idx = 0; insert_idx < total_pixel_rows && curr_row < IMAGE_ORIGIN_H; insert_idx++)
        {
            // 计算当前插入行的相对进度 (0.0 到 1.0)
            float progress = (total_pixel_rows > 1) ? (float)insert_idx / (total_pixel_rows - 1) : 0.0f;

            // 计算当前行的列偏移（线性插值）
            float current_col_offset = prev_accumulated_col + curr_col_offset * progress;
            int current_col_pixel = default_offset - (int)current_col_offset;

            // 边界处理
            if (current_col_pixel < 0)
            {
                int start_pos = -current_col_pixel;
                int copy_length = 128 - start_pos;
                if (copy_length > 0 && start_pos < 128)
                {
                    memcpy(&origin_map[curr_row][0],
                           &((vuint8 *)tmp_image[i])[start_pos],
                           copy_length);
                }
            }
            else if (current_col_pixel > IMAGE_ORIGIN_W - 128)
            {
                int copy_length = IMAGE_ORIGIN_W - current_col_pixel;
                if (copy_length > 0)
                {
                    memcpy(&origin_map[curr_row][current_col_pixel],
                           tmp_image[i],
                           copy_length);
                }
            }
            else
            {
                memcpy(&origin_map[curr_row][current_col_pixel],
                       tmp_image[i],
                       128);
            }

            curr_row++;
        }
    }

    // 释放内存
    circlular_queue_free_exported(tmp_image, CCD_OUTLOOK);
    circlular_queue_free_exported(tmp_distortion, CCD_OUTLOOK);
}