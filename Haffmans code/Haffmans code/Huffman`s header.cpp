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

// Генерация кодов Хаффмана
void generateHuffmanCodes(NODE* root, char codes[ALPHABET][MAX_CODE_SIZE], char* code, int level)
{
    if (!root)
    {
        return;
    }
    if (!root->left && !root->right)
    {
        code[level] = '\0';
        strcpy(codes[root->symb], code);
    }
    else
    {
        code[level] = '0';
        generateHuffmanCodes(root->left, codes, code, level + 1);
        code[level] = '1';
        generateHuffmanCodes(root->right, codes, code, level + 1);
    }
}

// Кодирование файла побитово
void encodeFile(FILE* inputFile, FILE* outputFile, char codes[ALPHABET][MAX_CODE_SIZE])
{
    /*int ch;
    while ((ch = fgetc(inputFile)) != EOF)
    {
        fputs(codes[ch], outputFile);
    }*/
    BIT2CHAR bitBuff = { 0 };
    int bitCnt = 0, ch;
    while ((ch = fgetc(inputFile)) != EOF)
    {
        for (char* p = codes[ch]; *p; p++)
        {
            if (*p == '1')
            {
                bitBuff.symb |= (1 << (7 - bitCnt));
            }
            bitCnt++;
            if (bitCnt == 8)
            {
                fputc(bitBuff.symb, outputFile);
                bitBuff.symb = 0;
                bitCnt = 0;
            }
        }
    }
    if (bitCnt > 0)
    {
        fputc(bitBuff.symb, outputFile);
    }
}

// Декодирование файла
void decodeFile(FILE* inputFile, FILE* outputFile, NODE* root)
{
    /*NODE* current = root;
    int ch;
    while ((ch = fgetc(inputFile)) != EOF) {
        if (ch == '0') {
            current = current->left;
        }
        else if (ch == '1') {
            current = current->right;
        }

        if (!current->left && !current->right) {
            fputc(current->symb, outputFile);
            current = root;
        }
    }*/
    NODE* current = root;
    int ch;
    while ((ch = fgetc(inputFile)) != EOF)
    {
        BIT2CHAR bitBuffer;
        bitBuffer.symb = ch;

        for (int i = 7; i >= 0; i--)
        {
            current = (bitBuffer.symb & (1 << i)) ? current->right : current->left;
            if (!current->left && !current->right)
            {
                fputc(current->symb, outputFile);
                current = root;
            }
        }
    }
}

void loadCodesFromFile(char codes[ALPHABET][MAX_CODE_SIZE], const char* filename)
{
    FILE* file = fopen(filename, "r");
    if (!file)
    {
        fprintf(stderr, "Error: cannot open codes file\n");
        exit(1);
    }
    unsigned char symbol;
    char code[MAX_CODE_SIZE];
    while (fscanf(file, "%c %s\n", &symbol, code) == 2)
    {
        strcpy(codes[symbol], code);
    }
    fclose(file);
}

void saveCodesToFile(const char codes[ALPHABET][MAX_CODE_SIZE], const char* filename)
{
    FILE* file = fopen(filename, "w");
    if (!file)
    {
        fprintf(stderr, "Error: Could not open %s\n", filename);
        return;
    }
    for (int i = 0; i < ALPHABET; i++)
    {
        if (codes[i][0] != '\0')
        {
            fprintf(file, "%c: %s\n", (char)i, codes[i]);
        }
    }
    fclose(file);
}

// Освобождение памяти дерева
void freeTree(NODE* root)
{
    if (root)
    {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}