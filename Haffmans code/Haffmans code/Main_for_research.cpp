#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Huffman`s header.h"
#include <time.h>

void measureExecutionTime(void (*func)(const char*, int*), const char* description, const char* filename, int* freq)
{
    clock_t start_time = clock();
    func(filename, freq);
    clock_t end_time = clock();
    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("%s time: %f seconds\n", description, time_taken);
}

void encode(const char* filename, int* freq)
{
    NODE* root = buildTree(freq);
    char codes[ALPHABET][MAX_CODE_SIZE] = { 0 };
    char tempCode[MAX_CODE_SIZE];
    generateHuffmanCodes(root, codes, tempCode, 0);

    FILE* fin = fopen(filename, "rb");
    FILE* fout = fopen("encoded.bin", "wb");
    encodeFile(fin, fout, codes);
    fclose(fin);
    fclose(fout);

    saveCodesToFile(codes, "codes.txt");
    freeTree(root);
}

void decode(const char* filename, int* freq)
{
    NODE* root = buildTree(freq);
    char codes[ALPHABET][MAX_CODE_SIZE] = { 0 };
    char tempCode[MAX_CODE_SIZE];
    generateHuffmanCodes(root, codes, tempCode, 0);

    FILE* f = fopen("encoded.bin", "rb");
    FILE* fout = fopen("decoded.txt", "wb");
    decodeFile(f, fout, root);
    fclose(f);
    fclose(fout);

    freeTree(root);
}

int main()
{
    int freq[ALPHABET] = { 0 };
    const char* filename = "war_and_peace.txt";
    FILE* fin = fopen(filename, "rb");
    if (!fin) 
    {
        fprintf(stderr, "Error: cannot open codes file\n");
        exit(1);
    }
    fseek(fin, 0L, SEEK_END);
    long int length = ftell(fin);
    fseek(fin, 0, SEEK_SET);
    for (int i = 0; i < length; i++)
    {
        freq[(unsigned char)fgetc(fin)]++;
    }
    fclose(fin);

    measureExecutionTime(encode, "Encoding", filename, freq);
    measureExecutionTime(decode, "Decoding", filename, freq);
    return 0;
}