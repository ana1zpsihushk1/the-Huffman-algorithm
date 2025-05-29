#include "New_Huff.h"
#include <iostream>
#include <string>

using namespace std;

int main() 
{
    Huffman huffman;
    string mode;
    cout << "Enter mode (compress / decompress): ";
    cin >> mode;

    if (mode == "compress") 
    {
        string inputFile, outputFile, codeFile;
        cout << "Enter input file to compress: ";
        cin >> inputFile;
        cout << "Enter output (encoded) file: ";
        cin >> outputFile;
        cout << "Enter file to save Huffman codes: ";
        cin >> codeFile;

        huffman.build(inputFile);
        huffman.generateCodes();
        huffman.saveCodes(codeFile);
        huffman.encodeFile(inputFile, outputFile);

        cout << "Compression complete.\n";
    }
    else if (mode == "decompress") 
    {
        string encodedFile, outputFile, codeFile;
        cout << "Enter encoded file: ";
        cin >> encodedFile;
        cout << "Enter output (decoded) file: ";
        cin >> outputFile;
        cout << "Enter file with Huffman codes: ";
        cin >> codeFile;

        huffman.loadCodes(codeFile);
        huffman.buildTreeFromCodes(); 
        huffman.decodeFile(encodedFile, outputFile);

        cout << "Decompression complete.\n";
    }
    else 
    {
        cerr << "Invalid mode.\n";
    }
    return 0;
}