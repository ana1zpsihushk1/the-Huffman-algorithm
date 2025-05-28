#pragma once
#include <memory>
#include <unordered_map>
#include <string>

using namespace std;

struct Node 
{
    unsigned char symbol;
    int frequency;
    shared_ptr<Node> left;
    shared_ptr<Node> right;

    Node(unsigned char sym, int freq)
        : symbol(sym), frequency(freq), left(nullptr), right(nullptr) {}

    bool isLeaf() const 
    {
        return !left && !right;
    }
};

class Huffman
{
public:
    void build(const string& inputFilePath);

    void generateCodes();
    void saveCodes(const string& codeFilePath) const;
    void loadCodes(const string& codeFilePath);

    void encodeFile(const string& inputFilePath, const string& encodedFilePath) const;
    
    void buildTreeFromCodes();
    void decodeFile(const string& encodedFilePath, const string& outputFilePath) const;

    void freeTree();
private:
    shared_ptr<Node> root;
    unordered_map<unsigned char, string> codes;

    void buildTree(const unordered_map<unsigned char, int>& freqMap);
    void generateCodesFromTree(shared_ptr<Node>& node, const string& code);
    void freeTreeHelper(std::shared_ptr<Node>& node);
};
