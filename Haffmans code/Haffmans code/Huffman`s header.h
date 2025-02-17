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

//�������(� ������������)
typedef struct Queue
{
    NODE* nodes[ALPHABET];
    int size;
} QUEUE;

// ������� �����
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

// ������� (� ������������)
QUEUE* createQueue();
void enQueue(QUEUE* queue, NODE* node);
NODE* deQueue(QUEUE* queue);

// ������ ��������
NODE* createNode(unsigned char symb, unsigned int freq);
NODE* MakeNodeFromNode(NODE* left, NODE* right);
void Add2List(NODE** head, NODE* newNode);
NODE* buildTree(int* freq);

// ����������� � �������������
void generateHuffmanCodes(NODE* root, char codes[ALPHABET][MAX_CODE_SIZE], char* code, int level);
void encodeFile(FILE* inputFile, FILE* outputFile, char codes[ALPHABET][MAX_CODE_SIZE]);
void decodeFile(FILE* inputFile, FILE* outputFile, NODE* root);

void loadCodesFromFile(char codes[ALPHABET][MAX_CODE_SIZE], const char* filename);
void saveCodesToFile(const char codes[ALPHABET][MAX_CODE_SIZE], const char* filename);

// ������������ ������
void freeTree(NODE* root);

#endif