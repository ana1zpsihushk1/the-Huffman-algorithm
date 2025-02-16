#include "Huffman`s header.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#define MAX_TREE_SIZE 256

// Очередь (с приоритетами)
QUEUE* createQueue()
{
    QUEUE* queue = (QUEUE*)malloc(sizeof(QUEUE));
    queue->size = 0; // Инициализация размера очереди
    return queue;
}

void enQueue(QUEUE* queue, NODE* node) // Добавление узла в очередь
{
    queue->nodes[queue->size++] = node;
}

NODE* deQueue(QUEUE* queue) // Извлечение узла с наименьшей частотой из очереди
{
    if (queue->size == 0)
    {
        return NULL;
    }
    NODE* min_node = queue->nodes[0];
    for (int i = 1; i < queue->size; i++)
    {
        queue->nodes[i - 1] = queue->nodes[i];
    }
    queue->size--;
    return min_node;
}
