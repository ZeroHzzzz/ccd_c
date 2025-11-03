#include "cir_queue.h"

circlular_queue_t *circlular_queue_init(vint16 capacity, size_t elem_size)
{
    if (capacity <= 0 || elem_size == 0)
        return NULL;

    circlular_queue_t *queue = malloc(sizeof(circlular_queue_t));
    if (!queue)
        return NULL;

    // 分配连续内存：capacity * elem_size 字节
    vuint8 *buffer = malloc(capacity * elem_size);
    if (!buffer)
    {
        free(queue);
        return NULL;
    }

    queue->buffer = buffer;
    queue->capacity = capacity;
    queue->elem_size = elem_size;
    queue->head = 0;
    queue->rear = 0;

    return queue;
}

void circlular_queue_free(circlular_queue_t *queue)
{
    if (queue)
    {
        free(queue->buffer);
        free(queue);
    }
}

/*
这是一个辅助函数，用于释放 circlular_queue_export 导出的内存
void **exported = circlular_queue_export(q);
circlular_queue_free_exported(exported, size);
*/
void circlular_queue_free_exported(void **exported, vint16 count)
{
    if (!exported)
        return;
    for (vint16 i = 0; i < count; i++)
    {
        free(exported[i]);
    }
    free(exported);
}

vuint8 circlular_queue_enqueue(circlular_queue_t *queue, const void *element)
{
    if (circlular_queue_full(queue))
        return 0;

    vuint8 *dest = queue->buffer + queue->rear * queue->elem_size;
    memcpy(dest, element, queue->elem_size);

    queue->rear = (queue->rear + 1) % queue->capacity;
    return 1;
}

vuint8 circlular_queue_dequeue(circlular_queue_t *queue, void *output)
{
    if (circlular_queue_empty(queue))
        return 0;

    vuint8 *src = queue->buffer + queue->head * queue->elem_size;
    memcpy(output, src, queue->elem_size);

    queue->head = (queue->head + 1) % queue->capacity;
    return 1;
}

vuint8 circlular_queue_update_windows(circlular_queue_t *queue, const void *element)
{
    // 错误检查：指针是否为空
    if (!queue || !element || !queue->buffer || queue->capacity == 0 || queue->elem_size == 0)
    {
        return 0; // 失败
    }

    vuint8 is_full = circlular_queue_full(queue);

    if (is_full)
    {
        // 队列满：丢弃最老元素
        queue->head = (queue->head + 1) % queue->capacity;
    }

    // 插入新元素到 rear 位置
    vuint8 *dest = queue->buffer + queue->rear * queue->elem_size;
    memcpy(dest, element, queue->elem_size);
    queue->rear = (queue->rear + 1) % queue->capacity;

    // 如果原本满，返回 1；否则返回 2
    return is_full ? 1 : 2;
}

// 正序导出，意味着最老的元素索引最小，在最上面
void **circlular_queue_export(circlular_queue_t *queue)
{
    // 用完记得调用 circlular_queue_free_exported 释放内存
    vint16 size = circlular_queue_size(queue);
    if (size == 0)
        return NULL;

    void **result = malloc(size * sizeof(void *));
    if (!result)
        return NULL;

    for (vint16 i = 0; i < size; i++)
    {
        vint16 idx = (queue->head + i) % queue->capacity;
        result[i] = malloc(queue->elem_size);
        if (!result[i])
        {
            for (vint16 j = 0; j < i; j++)
                free(result[j]);
            free(result);
            return NULL;
        }
        memcpy(result[i], queue->buffer + idx * queue->elem_size, queue->elem_size);
    }
    return result;
}

// 倒序导出，意味着最老的元素索引最大，在最下面。如果是图像就需要这个
void **circlular_queue_export_reverse(circlular_queue_t *queue)
{
    vint16 size = circlular_queue_size(queue);
    if (size == 0)
        return NULL;

    void **result = malloc(size * sizeof(void *));
    if (!result)
        return NULL;

    for (vint16 i = 0; i < size; i++)
    {
        vint16 idx = (queue->head + i) % queue->capacity;
        void *elem = malloc(queue->elem_size);
        if (!elem)
        {
            // 清理已分配的内存（注意：现在存的是 result[size-1-j]）
            for (vint16 j = 0; j < i; j++)
                free(result[size - 1 - j]);
            free(result);
            return NULL;
        }
        memcpy(elem, queue->buffer + idx * queue->elem_size, queue->elem_size);
        result[size - 1 - i] = elem; // 关键修改：逆序存放
    }
    return result;
}

vint16 circlular_queue_size(circlular_queue_t *queue)
{
    return (queue->rear - queue->head + queue->capacity) % queue->capacity;
}

vuint8 circlular_queue_empty(circlular_queue_t *queue)
{
    return queue->head == queue->rear;
}

vuint8 circlular_queue_full(circlular_queue_t *queue)
{
    return (queue->rear + 1) % queue->capacity == queue->head;
}