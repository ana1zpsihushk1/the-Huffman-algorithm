#ifndef HUFFMAN
#define HUFFMAN

#include <stdio.h>
#include <stdlib.h>

#define ALPHABET 256
#define MAX_CODE_SIZE 256

typedef struct Node
{
    unsigned char symb;
    unsigned int freq;
    struct Node* left, * right, * next;
}NODE;

//Очередь(с приоритетами)
typedef struct Queue
{
    NODE* nodes[ALPHABET];
    int size;
} QUEUE;

// Битовый буфер
typedef union bit2char
{
    char symb;
    struct bit
    {
        unsigned b1 : 1;
        unsigned b2 : 1;
        unsigned b3 : 1;
        unsigned b4 : 1;
        unsigned b5 : 1;
        unsigned b6 : 1;
        unsigned b7 : 1;
        unsigned b8 : 1;
    } mbit;
}BIT2CHAR;

// Очередь (с приоритетами)
QUEUE* createQueue();
void enQueue(QUEUE* queue, NODE* node);
NODE* deQueue(QUEUE* queue);

// Дерево Хаффмана
NODE* createNode(unsigned char symb, unsigned int freq);
NODE* MakeNodeFromNode(NODE* left, NODE* right);
void Add2List(NODE** head, NODE* newNode);
NODE* buildTree(int* freq);

#endif