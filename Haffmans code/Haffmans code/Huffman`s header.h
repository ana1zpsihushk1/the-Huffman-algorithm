#ifndef HUFFMAN
#define HUFFMAN

typedef struct Node {
	char value;
	int freq;
	struct Node *left, *right;
}NODE;

void createNode(const NODE* mas,int rel);

#endif