#ifndef _ALGO_CIR_QUEUE_H
#define _ALGO_CIR_QUEUE_H

#include "zf_common_typedef.h"

typedef struct
{
    vint16 capacity;     // 队列的实际数组大小 (N)
    vint16 head;         // 队头索引 (指向队头元素)
    vint16 rear;         // 队尾索引 (指向队尾元素的下一个位置)
    vuint16 **array;     // 存储元素（元素本身是数组）的二维指针
    vint16 element_size; // 每个元素数组的大小
} circlular_queue_array_t;

circlular_queue_array_t *circlular_queue_array_init(vint16 capacity, vint16 element_size);
void circlular_queue_array_free(circlular_queue_array_t *queue);
vuint8 circlular_queue_array_empty(circlular_queue_array_t *queue);
vuint8 circlular_queue_array_full(circlular_queue_array_t *queue);
vuint8 circlular_queue_array_enqueue(circlular_queue_array_t *queue, const vuint16 *element);
vuint8 circlular_queue_array_dequeue(circlular_queue_array_t *queue);
vuint8 circlular_queue_array_update_windows(circlular_queue_array_t *queue, const vuint16 *element);
vint16 circlular_queue_array_size(circlular_queue_array_t *queue);
vuint16 **circlular_queue_array_export(circlular_queue_array_t *queue);

#endif