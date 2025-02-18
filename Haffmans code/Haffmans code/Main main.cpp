#define _CRT_SECURE_NO_WARNINGS
#include "Huffman`s header.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main()
{
    int freq[ALPHABET] = { 0 };

    FILE* fin = fopen("war_and_peace.txt", "rb");
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

    NODE* root = buildTree(freq);
    if (!root) {
        fprintf(stderr, "Error: failed to build Huffman tree\n");
        return -1;
    }

    char codes[ALPHABET][MAX_CODE_SIZE] = { 0 };
    char tempCode[MAX_CODE_SIZE];
    generateHuffmanCodes(root, codes, tempCode, 0);

    saveCodesToFile(codes, "codes.txt");

    fin = fopen("war_and_peace.txt", "rb");
    FILE* fout = fopen("encoded.bin", "wb");
    encodeFile(fin, fout, codes);
    fclose(fin);
    fclose(fout);

    char loadedCodes[ALPHABET][MAX_CODE_SIZE] = { 0 };
    loadCodesFromFile(loadedCodes, "codes.txt");

    fin = fopen("encoded.bin", "rb");
    fout = fopen("decoded.txt", "wb");
    decodeFile(fin, fout, root);
    fclose(fin);
    fclose(fout);

    freeTree(root);
    return 0;
}