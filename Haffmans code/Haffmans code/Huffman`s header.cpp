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

NODE* createNode(unsigned char symb, unsigned int freq) // Создание узла
{
    NODE* node = (NODE*)malloc(sizeof(NODE));
    node->symb = symb;
    node->freq = freq;
    node->left = node->right = NULL;
    node->next = NULL;
    return node;
}

NODE* MakeNodeFromNode(NODE* left, NODE* right) // Объединение узлов
{
    NODE* parent = createNode(0, left->freq + right->freq);
    parent->left = left;
    parent->right = right;
    return parent;
}

// Добавление в список
void Add2List(NODE** head, NODE* newNode)
{
    if (!newNode)
    {
        return;
    }
    if (*head == NULL || newNode->freq < (*head)->freq)
    {
        newNode->next = *head;
        *head = newNode;
        return;
    }
    NODE* current = *head;
    while (current->next && newNode->freq > current->next->freq)
    {
        current = current->next;
    }
    newNode->next = current->next;
    current->next = newNode;
}

// Построение дерева
NODE* buildTree(int* freq)
{
    NODE* head = NULL;
    for (int i = 0; i < ALPHABET; i++)
    {
        if (freq[i] > 0)
        {
            NODE* node = createNode((unsigned char)i, freq[i]);
            Add2List(&head, node);
        }
    }
    while (head && head->next)
    {
        NODE* left = head;
        NODE* right = head->next;
        head = right->next;
        NODE* parent = MakeNodeFromNode(left, right);
        Add2List(&head, parent);
    }
    return head;
}
