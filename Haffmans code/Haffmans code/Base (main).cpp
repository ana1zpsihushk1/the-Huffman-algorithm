#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include "Huffman`s header.h"

int main()
{
	FILE* fin = fopen("input.txt", "r");
	if (!fin)
	{
		fprintf(stderr, "Can`t open dile for reading\n");
		return -1;
	}

	FILE* fout = fopen("output.txt", "w");
	if (!fout)
	{
		fprintf(stderr, "Can`t open dile for reading\n");
		return -2;
	}

	fclose(fin);
	fclose(fout);
	return 0;
}