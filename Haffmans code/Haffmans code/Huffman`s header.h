#ifndef HUFFMAN
#define HUFFMAN

#define MAX_TREE_SIZE 256

typedef struct Node {
	char value;
	int freq;
	struct Node *left, *right;
}NODE;

typedef struct Queue{
	NODE* nodes[MAX_TREE_SIZE];
	int size;
}QUEUE;

void readFile(const char* infile);

#endif