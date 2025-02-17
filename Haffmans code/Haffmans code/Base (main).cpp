#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "Huffman`s header.h"
#include <time.h>

void measureExecutionTime(void (*func)())
{
    clock_t start_time = clock();
    func();
    clock_t end_time = clock();
    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Time: %f seconds\n", time_taken);
}

void runMain()
{
    // Функция, время которой измеряем
}

int main()
{
    int freq[ALPHABET] = { 0 };

    // Читаем частоты символов
    FILE* fin = fopen("input.txt", "rb");
    if (!fin) {
        fprintf(stderr, "Can't open input file\n");
        return -1;
    }

    fseek(fin, 0L, SEEK_END);
    long int length = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    for (int i = 0; i < length; i++) {
        freq[(unsigned char)fgetc(fin)]++;
    }
    fclose(fin);

    //measureExecutionTime(runMain);

    // Создаем дерево
    NODE* root = buildTree(freq);
    if (!root) {
        fprintf(stderr, "Error: failed to build Huffman tree\n");
        return -1;
    }

    // Генерируем коды
    char codes[ALPHABET][MAX_CODE_SIZE] = { 0 };
    char tempCode[MAX_CODE_SIZE];
    //generateHuffmanCodes(root, codes, tempCode, 0);

    // Кодируем файл
    fin = fopen("input.txt", "rb");
    FILE* fout = fopen("encoded.txt", "wb"); //было bin
    encodeFile(fin, fout, codes);
    fclose(fin);
    fclose(fout);

    // Декодируем файл
    fin = fopen("encoded.txt", "rb");
    fout = fopen("decoded.txt", "wb");
    //decodeFile(fin, fout, root);
    fclose(fin);
    fclose(fout);

    freeTree(root);
    return 0;
}