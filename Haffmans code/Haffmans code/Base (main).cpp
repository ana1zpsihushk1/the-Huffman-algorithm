#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "Huffman`s header.h"
#include <time.h>

void measureExecutionTime(void (*func)(), const char* description)
{
    clock_t start_time = clock();
    func();
    clock_t end_time = clock();
    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("%s time: %f seconds\n", description, time_taken);
}

void encode()
{
    int freq[ALPHABET] = { 0 };
    FILE* fin = fopen("20mb-examplefile-com.txt", "rb");
    fseek(fin, 0L, SEEK_END);
    long int length = ftell(fin);
    fseek(fin, 0, SEEK_SET);

    for (int i = 0; i < length; i++) {
        freq[(unsigned char)fgetc(fin)]++;
    }
    fclose(fin);

    NODE* root = buildTree(freq);
    char codes[ALPHABET][MAX_CODE_SIZE] = { 0 };
    char tempCode[MAX_CODE_SIZE];
    generateHuffmanCodes(root, codes, tempCode, 0);

    fin = fopen("20mb-examplefile-com.txt", "rb");
    FILE* fout = fopen("encoded.bin", "wb");
    encodeFile(fin, fout, codes);
    fclose(fin);
    fclose(fout);

    saveCodesToFile(codes, "codes.txt");
    freeTree(root);
}

void decode()
{
    char loadedCodes[ALPHABET][MAX_CODE_SIZE] = { 0 };
    loadCodesFromFile(loadedCodes, "codes.txt");

    int freq[ALPHABET] = { 0 };
    for (int i = 0; i < ALPHABET; i++) {
        if (loadedCodes[i][0] != '\0') {
            freq[i] = 1;
        }
    }
    NODE* root = buildTree(freq);

    FILE* fin = fopen("encoded.bin", "rb");
    FILE* fout = fopen("decoded.txt", "wb");
    decodeFile(fin, fout, root);
    fclose(fin);
    fclose(fout);

    freeTree(root);
}

int main()
{
    measureExecutionTime(encode, "Encoding");
    measureExecutionTime(decode, "Decoding");
    return 0;
}