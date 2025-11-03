#ifndef _ALGO_CIR_QUEUE_H
#define _ALGO_CIR_QUEUE_H

#include "zf_common_typedef.h"

typedef struct
{
    uint8_t *buffer;  // 一整块字节缓冲区
    vint16 capacity;  // 最多可存多少个“元素”
    size_t elem_size; // 每个元素占多少字节（关键！）
    vint16 head;      // 队首索引（单位：元素个数）
    vint16 rear;      // 队尾索引（单位：元素个数）
} circlular_queue_t;

// 初始化：capacity=元素个数，elem_size=每个元素多少字节
circlular_queue_t *circlular_queue_init(vint16 capacity, size_t elem_size);

void circlular_queue_free(circlular_queue_t *queue);

// 入队：element 指向一个 elem_size 大小的数据
vuint8 circlular_queue_enqueue(circlular_queue_t *queue, const void *element);

// 出队：output 必须指向一个 elem_size 大小的缓冲区
vuint8 circlular_queue_dequeue(circlular_queue_t *queue, void *output);

// 如果满，先出队再入队（滑动窗口）
vuint8 circlular_queue_update_windows(circlular_queue_t *queue, const void *element);

// 导出所有元素（返回 void**，每个指针指向一个 elem_size 数据）
void **circlular_queue_export(circlular_queue_t *queue);
void **circlular_queue_export_reverse(circlular_queue_t *queue);

// 工具函数
vint16 circlular_queue_size(circlular_queue_t *queue);
vuint8 circlular_queue_empty(circlular_queue_t *queue);
vuint8 circlular_queue_full(circlular_queue_t *queue);

#endif