#include "test.h"

void test_handler()
{
    test_encoder();
}

void test_queue()
{
    circlular_queue_t *q_int = circlular_queue_init(5, sizeof(int));

    int a = 42, b = 100;
    circlular_queue_enqueue(q_int, &a);
    circlular_queue_enqueue(q_int, &b);

    int out;
    circlular_queue_dequeue(q_int, &out);
    printf("Dequeued int: %d\n", out); // 42

    circlular_queue_t *q_array = circlular_queue_init(3, 4 * sizeof(int));

    int arr1[4] = {1, 2, 3, 4};
    int arr2[4] = {5, 6, 7, 8};

    circlular_queue_enqueue(q_array, arr1); // 数组名 = 首地址
    circlular_queue_enqueue(q_array, arr2);

    int out_arr[4];
    circlular_queue_dequeue(q_array, out_arr);
    printf("Array: %d %d %d %d\n", out_arr[0], out_arr[1], out_arr[2], out_arr[3]);
}

void test_encoder()
{
    while (1)
    {
        lcd_show_int(0, 0, encoder_get_data(&encoder_left), 5);
        system_delay_ms(30);
    }
}