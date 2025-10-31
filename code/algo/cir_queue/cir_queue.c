#include "cir_queue.h"

circlular_queue_array_t *circlular_queue_array_init(vint16 capacity, vint16 element_size)
{
    if (capacity <= 1)
    {
        return NULL;
    }

    circlular_queue_array_t *queue = (circlular_queue_array_t *)malloc(sizeof(circlular_queue_array_t));
    if (!queue)
    {
        return NULL;
    }

    queue->capacity = capacity;
    queue->element_size = element_size;
    queue->head = 0; // 初始化 front 和 rear 在同一个位置（队空）
    queue->rear = 0;

    // 分配存储 element_size 大小数组的指针数组
    queue->array = (vuint16 **)malloc(capacity * sizeof(vuint16 *));
    if (!queue->array)
    {
        free(queue);
        return NULL;
    }

    // 为每个元素分配实际的数组内存
    for (int i = 0; i < capacity; i++)
    {
        queue->array[i] = (vuint16 *)malloc(queue->element_size * sizeof(vuint16));
        if (!queue->array[i])
        {
            for (int j = 0; j < i; j++)
            {
                free(queue->array[j]);
            }
            free(queue->array);
            free(queue);
            return NULL;
        }
    }

    return queue;
}

void circlular_queue_array_free(circlular_queue_array_t *queue)
{
    if (!queue)
        return;

    if (queue->array)
    {
        for (int i = 0; i < queue->capacity; i++)
        {
            free(queue->array[i]);
        }
        free(queue->array);
    }
    free(queue);
}

/**
 * 将一个元素数组插入到队列尾部 (入队)
 * @param queue 队列指针
 * @param element 要入队的整数数组的指针
 * @return 如果入队成功返回 true，否则（队列已满）返回 false
 */
vuint8 circlular_queue_array_enqueue(circlular_queue_array_t *queue, const vuint16 *element)
{
    if (circlular_queue_array_full(queue))
    {
        return 0;
    }

    memcpy(queue->array[queue->rear], element, queue->element_size * sizeof(vuint16));
    // int old_rear = queue->rear;
    queue->rear = (queue->rear + 1) % queue->capacity;

    return 1;
}

/**
 * 从队列头部删除并返回一个元素数组 (出队)
 * @param queue 队列指针
 * @return 队头元素数组的指针；如果队列为空，返回 0
 */
vuint8 circlular_queue_array_dequeue(circlular_queue_array_t *queue)
{
    if (circlular_queue_array_empty(queue))
    {
        return 0;
    }

    // int *element = queue->array[queue->head];
    // int old_head = queue->head;
    queue->head = (queue->head + 1) % queue->capacity;

    return 1;
}

vuint8 circlular_queue_array_update_windows(circlular_queue_array_t *queue, const vuint16 *element)
{
    if (circlular_queue_array_full(queue))
    {
        circlular_queue_array_dequeue(queue);          // 先出队最老的
        circlular_queue_array_enqueue(queue, element); // 入队最新的
    }
}

vuint16 **circlular_queue_array_export(circlular_queue_array_t *queue)
{
    vuint16 size = circlular_queue_array_size(queue);
    if (size == 0)
        return NULL;

    vuint16 **exported_array = (vuint16 **)malloc(queue->capacity * sizeof(vuint16 *));
    if (!exported_array)
    {
        return NULL;
    }

    int curr_idx = queue->head; // 从 head 开始遍历。由于队头是最老的，根据图像可以知道他应该在最下面
    for (vuint16 i = 0; i < size; i++)
    {
        exported_array[queue->capacity - 1 - i] = (vuint16 *)malloc(queue->element_size * sizeof(vuint16));
        if (!exported_array[queue->capacity - 1 - i])
        {
            for (vuint16 j = 0; j < i; j++)
            {
                free(exported_array[size - 1 - j]);
            }
            free(exported_array);
            return NULL;
        }
        memcpy(exported_array[queue->capacity - 1 - i], queue->array[curr_idx], queue->element_size * sizeof(vuint16));
        curr_idx = (curr_idx + 1) % queue->capacity;
    }

    return exported_array;
}

/**
 * 获取队列元素个数
 * @param queue 队列指针
 * @return 元素个数
 */
vint16 circlular_queue_array_size(circlular_queue_array_t *queue)
{
    return (queue->rear - queue->head + queue->capacity) % queue->capacity;
}

/**
 * 检查队列是否为空
 * 队空条件：head == rear
 * @param queue 队列指针
 * @return 如果队列为空返回 true，否则返回 false
 */
vuint8 circlular_queue_array_empty(circlular_queue_array_t *queue)
{
    return queue->head == queue->rear;
}

/**
 * 检查队列是否已满
 * 队满条件：(rear + 1) % capacity == head
 * @param queue 队列指针
 * @return 如果队列已满返回 true，否则返回 false
 */
vuint8 circlular_queue_array_full(circlular_queue_array_t *queue)
{
    return (queue->rear + 1) % queue->capacity == queue->head;
}
