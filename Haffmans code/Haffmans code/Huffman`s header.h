#ifndef HUFFMAN
#define HUFFMAN

struct Node {
	char character;
	int freq;
	struct Node* l, * r;
};

#endif