/*********************************************************************************************************************
 * TC264 Opensourec Library 即（TC264 开源库）是一个基于官方 SDK 接口的第三方开源库
 * Copyright (c) 2022 SEEKFREE 逐飞科技
 *
 * 本文件是 TC264 开源库的一部分
 *
 * TC264 开源库 是免费软件
 * 您可以根据自由软件基金会发布的 GPL（GNU General Public License，即 GNU通用公共许可证）的条款
 * 即 GPL 的第3版（即 GPL3.0）或（您选择的）任何后来的版本，重新发布和/或修改它
 *
 * 本开源库的发布是希望它能发挥作用，但并未对其作任何的保证
 * 甚至没有隐含的适销性或适合特定用途的保证
 * 更多细节请参见 GPL
 *
 * 您应该在收到本开源库的同时收到一份 GPL 的副本
 * 如果没有，请参阅<https://www.gnu.org/licenses/>
 *
 * 额外注明：
 * 本开源库使用 GPL3.0 开源许可证协议 以上许可申明为译文版本
 * 许可申明英文版在 libraries/doc 文件夹下的 GPL3_permission_statement.txt 文件中
 * 许可证副本在 libraries 文件夹下 即该文件夹下的 LICENSE 文件
 * 欢迎各位使用并传播本程序 但修改内容时必须保留逐飞科技的版权声明（即本声明）
 *
 * 文件名称          cpu0_main
 * 公司名称          成都逐飞科技有限公司
 * 版本信息          查看 libraries/doc 文件夹内 version 文件 版本说明
 * 开发环境          ADS v1.10.2
 * 适用平台          TC264D
 * 店铺链接          https://seekfree.taobao.com/
 *
 * 修改记录
 * 日期              作者                备注
 * 2022-09-15       pudding            first version
 ********************************************************************************************************************/
#include "zf_common_headfile.h"
#include "binary.h"
#include "imu.h"
#include "attitude.h"
#pragma section all "cpu0_dsram"
// 将本语句与#pragma section all restore语句之间的全局变量都放在CPU0的RAM中

// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设
// 本例程是开源库空工程 可用作移植或者测试各类内外设
vuint16 rate[128];
// **************************** 代码区域 ****************************
int core0_main(void)
{
    clock_init(); // 获取时钟频率<务必保留>
    debug_init(); // 初始化默认调试串口
    // 此处编写用户代码 例如外设初始化代码等

    device_init();

    // 此处编写用户代码 例如外设初始化代码等
    cpu_wait_event_ready(); // 等待所有核心初始化完毕

    while (TRUE)
    {
        imu_data_t imu_data = imu_get_tmp_data();
        tft180_show_float(0, 0, imu_data.gyro.x, 3, 3);
        tft180_show_float(0, 16, imu_data.gyro.y, 3, 3);
        tft180_show_float(0, 32, imu_data.gyro.z, 3, 3);
        tft180_show_float(0, 48, imu_data.accel.x, 3, 3);
        tft180_show_float(0, 64, imu_data.accel.y, 3, 3);
        tft180_show_float(0, 80, imu_data.accel.z, 3, 3);
        tft180_show_float(0, 96, attitude_get_pitch(), 3, 3);
        tft180_show_float(0, 112, attitude_get_roll(), 3, 3);
        tft180_show_float(0, 128, attitude_get_yaw(), 3, 3);
        if (tsl1401_finish_flag)
        {
            // 此处编写需要循环执行的代码
            // tft180_show_uint(0, 0, tsl1401_data[0][50], 4);
            // tft180_clear();
            // binary_ccd_simple(tsl1401_data[0], rate, 128);
            // tft180_show_wave(0, 16, rate, 128, 256, 128, 100);
            // tft180_show_wave(0, 0, rate, 128, 256, 160, 127);
            tsl1401_finish_flag = 0;

            // 此处编写需要循环执行的代码
        }
        system_delay_ms(30);
    }
}

#pragma section all restore
// **************************** 代码区域 ****************************
