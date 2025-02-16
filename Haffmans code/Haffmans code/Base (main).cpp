#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "Huffman`s header.h"

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