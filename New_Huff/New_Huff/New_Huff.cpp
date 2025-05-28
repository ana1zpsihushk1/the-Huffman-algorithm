#include "New_Huff.h"
#include <queue>
#include <bitset>
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

void Huffman::build(const string& inputFilePath)
{
    ifstream file(inputFilePath, ios::binary);
    unordered_map<unsigned char, int> freq;

    unsigned char byte;
    while (file.read(reinterpret_cast<char*>(&byte), 1))
    {
        freq[byte]++;
    }
    file.close();

    buildTree(freq);
}

void Huffman::buildTree(const unordered_map<unsigned char, int>& freqMap)
{
    if (freqMap.empty()) {
        std::cerr << "Error: input file contains no data.\n";
        root = nullptr;
        return;
    }

    auto cmp = [](const shared_ptr<Node>& a, const shared_ptr<Node>& b)
    {
        return a->frequency > b->frequency;
    };

    priority_queue<shared_ptr<Node>, vector<shared_ptr<Node>>, decltype(cmp)> pq(cmp);

    for (auto it = freqMap.begin(); it != freqMap.end(); ++it)
    {
        unsigned char sym = it->first;
        int fr = it->second;
        pq.push(make_shared<Node>(sym, fr));
    }

    if (pq.empty()) {
        root = nullptr;
        return;
    }

    if (pq.size() == 1) {
        auto only = pq.top(); pq.pop();
        root = make_shared<Node>(0, only->frequency);
        root->left = only;
        return;
    }

    while (pq.size() > 1)
    {
        auto left = pq.top(); pq.pop();
        auto right = pq.top(); pq.pop();
        auto parent = make_shared<Node>(0, left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }

    root = pq.top();
}

void Huffman::generateCodes()
{
    codes.clear();
    if (!root) {
        std::cerr << "Error: cannot generate codes. Tree not built.\n";
        return;
    }
    generateCodesFromTree(root, "");
    cout << "[generateCodes] Done. Codes count: " << codes.size() << endl;
}

void Huffman::generateCodesFromTree(shared_ptr<Node>& node, const string& code)
{
    if (!node) return;

    if (node->isLeaf()) {
        codes[node->symbol] = code.empty() ? "0" : code;
        return;
    }

    generateCodesFromTree(node->left, code + "0");
    generateCodesFromTree(node->right, code + "1");
}

void Huffman::saveCodes(const string& filename) const
{
    if (codes.empty()) {
        cerr << "Cannot save codes: codes table is empty.\n";
        return;
    }
    ofstream out(filename);
    for (const auto& pair : codes)
    {
        out << static_cast<int>(pair.first) << " " << pair.second << "\n";
    }
}


void Huffman::loadCodes(const string& filename)
{
    codes.clear();
    ifstream in(filename);
    int byteInt;
    string code;
    while (in >> byteInt >> code)
    {
        codes[static_cast<unsigned char>(byteInt)] = code;
    }
    cout << "[loadCodes] Loaded " << codes.size() << " codes" << endl;
}

void Huffman::encodeFile(const string& inputPath, const string& outputPath) const
{
    if (codes.empty()) {
        cerr << "Error: codes table is empty.\n";
        return;
    }

    ifstream in(inputPath, ios::binary);
    ofstream out(outputPath, ios::binary);

    if (!in.is_open() || !out.is_open()) {
        cerr << "Failed to open input or output file.\n";
        return;
    }

    unsigned char byte;
    string bitString;
    while (in.read(reinterpret_cast<char*>(&byte), 1))
    {
        auto it = codes.find(byte);
        if (it == codes.end()) {
            cerr << "Warning: no code for byte " << static_cast<int>(byte) << ". Skipped.\n";
            continue;
        }
        bitString += it->second;
    }

    if (bitString.empty()) {
        cerr << "No data encoded. Check if codes were generated correctly.\n";
        return;
    }

    while (bitString.size() % 8 != 0) bitString += '0';

    for (size_t i = 0; i < bitString.size(); i += 8)
    {
        bitset<8> b(bitString.substr(i, 8));
        out.put(static_cast<unsigned char>(b.to_ulong()));
    }
}

void Huffman::buildTreeFromCodes() {
    root = std::make_shared<Node>(0, 0);
    for (auto it = codes.begin(); it != codes.end(); ++it) {
        unsigned char ch = it->first;
        const std::string& code = it->second;

        auto node = root;
        for (char bit : code) {
            if (bit == '0') {
                if (!node->left) node->left = std::make_shared<Node>(0, 0);
                node = node->left;
            }
            else if (bit == '1') {
                if (!node->right) node->right = std::make_shared<Node>(0, 0);
                node = node->right;
            }
        }
        node->symbol = ch;
        node->frequency = 1; // 
    }
    cout << "[buildTreeFromCodes] Root created: " << (root ? "yes" : "no") << endl;
}

void Huffman::decodeFile(const string& inputPath, const string& outputPath) const
{
    if (!root) 
    {
        cerr << "Cannot decode — tree not built.\n";
        return;
    }
    ifstream in(inputPath, ios::binary);
    ofstream out(outputPath, ios::binary);

    string bitString;
    unsigned char byte;
    while (in.read(reinterpret_cast<char*>(&byte), 1))
    {
        bitset<8> b(byte);
        bitString += b.to_string();
    }

    shared_ptr<Node> node = root;
    for (char bit : bitString)
    {
        if (!node) break;
        node = (bit == '0') ? node->left : node->right;
        if (node->isLeaf()) {
            out.put(static_cast<char>(node->symbol));
            node = root;
        }
    }
}

void Huffman::freeTree()
{
    freeTreeHelper(root);
    root.reset();
}

void Huffman::freeTreeHelper(shared_ptr<Node>& node)
{
    if (!node) return;
    freeTreeHelper(node->left);
    freeTreeHelper(node->right);
    node.reset();
}