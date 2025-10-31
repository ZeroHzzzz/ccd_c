#include "imu.h"
#include "adis.h"

float gyroOffset[3] = {0.0f, 0.0f, 0.0f};

static imu_device_enum curr_device;
static vuint8 imu_state = 1; // 表示初始化状态
static imu_data_t tmp_data = {0};
const float rotation_matrix[3][3] = {
    {0, 1, 0},
    {1, 0, 0},
    {0, 0, 1}};

void imu_init(imu_device_enum device)
{
    curr_device = device;

    switch (curr_device)
    {
    case IMU_DEVICE_660RA:
        imu_state = imu660ra_init();
        break;

    case IMU_DEVICE_660RB:
        imu_state = imu660rb_init();
        break;

    case IMU_DEVICE_963RA:
        imu_state = imu963ra_init();
        break;

    case IMU_DEVICE_ADIS16505:
        imu_state = adis_init();
        break;

    default:
        imu_state = 1;
        break;
    }

    if (!imu_state) // 0 表示初始化成功, 1 表示初始化失败
    {
        // handler logic
        printf("IMU SUCCESS.\n");
    }
    else
    {
        // handler logic
        printf("IMU FAILED.\n");
    }
}

imu_data_t imu_get_data(void)
{
    imu_data_t data = {0}; // 初始化为0

    if (imu_state)
    {
        printf("IMU device not initialized.\n");
        return data; // 返回默认值
    }

    switch (curr_device)
    {
    case IMU_DEVICE_660RA:
        imu660ra_get_acc();
        imu660ra_get_gyro();

        data.accel.x = imu660ra_acc_transition(imu660ra_acc_x) * 9.7936f;
        data.accel.y = imu660ra_acc_transition(imu660ra_acc_y) * 9.7936f;
        data.accel.z = imu660ra_acc_transition(imu660ra_acc_z) * 9.7936f;

        data.gyro.x = imu660ra_gyro_transition(imu660ra_gyro_x) * 0.0174533f;
        data.gyro.y = imu660ra_gyro_transition(imu660ra_gyro_y) * 0.0174533f;
        data.gyro.z = imu660ra_gyro_transition(imu660ra_gyro_z) * 0.0174533f;
        break;

    case IMU_DEVICE_660RB:
        imu660rb_get_acc();
        imu660rb_get_gyro();

        data.accel.x = imu660rb_acc_transition(imu660rb_acc_x) * 9.7936f;
        data.accel.y = imu660rb_acc_transition(imu660rb_acc_y) * 9.7936f;
        data.accel.z = imu660rb_acc_transition(imu660rb_acc_z) * 9.7936f;

        data.gyro.x = imu660rb_gyro_transition(imu660rb_gyro_x) * 0.0174533f;
        data.gyro.y = imu660rb_gyro_transition(imu660rb_gyro_y) * 0.0174533f;
        data.gyro.z = imu660rb_gyro_transition(imu660rb_gyro_z) * 0.0174533f;
        break;

    case IMU_DEVICE_963RA:
        imu963ra_get_acc();
        imu963ra_get_gyro();

        data.accel.x = imu963ra_acc_transition(imu963ra_acc_x) * 9.7936f;
        data.accel.y = imu963ra_acc_transition(imu963ra_acc_y) * 9.7936f;
        data.accel.z = imu963ra_acc_transition(imu963ra_acc_z) * 9.7936f;

        data.gyro.x = imu963ra_gyro_transition(imu963ra_gyro_x) * 0.0174533f;
        data.gyro.y = imu963ra_gyro_transition(imu963ra_gyro_y) * 0.0174533f;
        data.gyro.z = imu963ra_gyro_transition(imu963ra_gyro_z) * 0.0174533f;
        break;

    case IMU_DEVICE_ADIS16505:
        adis_read_data(&data);
        break;

    default:
        printf("Unknown IMU device.\n");
        break;
    }

    imu_rotate(&data);
    tmp_data = data;

    return data;
}

void imu_init_offset()
{
    imu_data_t data;
    for (int i = 0; i < 4000; i++)
    {
        data = imu_get_data();
        if (fabsf(data.gyro.x) + fabsf(data.gyro.y) + fabsf(data.gyro.z) >
            gyroscope_threshold)
        {
            i--;
            continue;
        }
        gyroOffset[0] += data.gyro.x;
        gyroOffset[1] += data.gyro.y;
        gyroOffset[2] += data.gyro.z;
        system_delay_ms(1);
    }
    gyroOffset[0] *= 0.0005f;
    gyroOffset[1] *= 0.0005f;
    gyroOffset[2] *= 0.0005f;
}

void imu_remove_offset(imu_data_t *data)
{
    data->gyro.x -= gyroOffset[0];
    data->gyro.y -= gyroOffset[1];
    data->gyro.z -= gyroOffset[2];
}

imu_data_t imu_get_tmp_data()
{
    return tmp_data;
}

void imu_rotate(imu_data_t *data)
{
    imu_data_t rotated_data;
    rotated_data.accel.x = rotation_matrix[0][0] * data->accel.x + rotation_matrix[0][1] * data->accel.y + rotation_matrix[0][2] * data->accel.z;
    rotated_data.accel.y = rotation_matrix[1][0] * data->accel.x + rotation_matrix[1][1] * data->accel.y + rotation_matrix[1][2] * data->accel.z;
    rotated_data.accel.z = rotation_matrix[2][0] * data->accel.x + rotation_matrix[2][1] * data->accel.y + rotation_matrix[2][2] * data->accel.z;

    rotated_data.gyro.x = rotation_matrix[0][0] * data->gyro.x + rotation_matrix[0][1] * data->gyro.y + rotation_matrix[0][2] * data->gyro.z;
    rotated_data.gyro.y = rotation_matrix[1][0] * data->gyro.x + rotation_matrix[1][1] * data->gyro.y + rotation_matrix[1][2] * data->gyro.z;
    rotated_data.gyro.z = rotation_matrix[2][0] * data->gyro.x + rotation_matrix[2][1] * data->gyro.y + rotation_matrix[2][2] * data->gyro.z;

    *data = rotated_data;
}

void imu_reset(void)
{
    imu_state = 1;
    imu_init(curr_device);
    memset(gyroOffset, 0, sizeof(gyroOffset));
    tmp_data = (imu_data_t){0};
}