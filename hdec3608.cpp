// Min Zeng     cs610 3608 prp

#include <iostream>
#include <fstream>
#include <vector>
#include "Huffman3608.h"
#include "MinHeap3608.h"

using namespace std;

bool readEncodedFile3608(const char* filePath, vector<Node3608*> &minHeap, vector<unsigned char> &encodedData, char &lastByteBitCount, size_t &originFileSize) {
    ifstream fin(filePath, ios::binary);
    if (fin.fail()) {
        return false;
    }

    streampos fsize = fin.tellg();
    fin.seekg(0, ios::end);
    fsize = fin.tellg() - fsize;
    fin.clear();
    fin.seekg(0, ios::beg);

    size_t size;
    fin.read(reinterpret_cast<char*>(&size), sizeof(size));
    fsize -= sizeof(size);

    minHeap.resize(size);

    unsigned char bytes[256];
    int counts[256];
    fin.read(reinterpret_cast<char*>(&bytes[0]), size * sizeof(unsigned char));
    fin.read(reinterpret_cast<char*>(&counts[0]), size * sizeof(int));
    fsize -= size * sizeof(unsigned char);
    fsize -= size * sizeof(int);

    Node3608* node;
    for (int i = 0; i < size; i++) {
        node = new Node3608(bytes[i], counts[i]);
        minHeap[i] = node;
    }

    fin.read(reinterpret_cast<char *>(&originFileSize), sizeof(originFileSize));
    fsize -= sizeof(originFileSize);
    fin.read(reinterpret_cast<char*>(&lastByteBitCount), sizeof(lastByteBitCount));
    fsize -= sizeof(lastByteBitCount);

    encodedData.resize(fsize);

    fin.read(reinterpret_cast<char*>(&encodedData[0]), fsize * sizeof(unsigned char));
    fin.close();
    
    return true;
}

void writeDecodedFile3608(const char* filePath, vector<unsigned char> decodedData) {
    string decodedPath = string(filePath);
    decodedPath = decodedPath.substr(0, decodedPath.size()-4);
    
    ofstream fout(decodedPath.c_str(), ios::out | ios::binary);
    fout.write(reinterpret_cast<char*>(&decodedData[0]), decodedData.size() * sizeof(unsigned char));
    fout.close();
    
    printf("Output file: %s\n", decodedPath.c_str());
}

vector<unsigned char> decode3608(vector<unsigned char> encodedData, Node3608* huffmanRoot, char lastByteBitCount, size_t originFileSize) {
    vector<unsigned char> decodedData = vector<unsigned char>(originFileSize);
    
    unsigned char nextByte = 0;
    Node3608* node = huffmanRoot;
    
    size_t pos = 0;
    for (int i = 0; i < encodedData.size(); i++) {
        nextByte = encodedData[i];
        for (int j = 0; j < 8; j++) {
            if (i == encodedData.size()-1 && lastByteBitCount > 0) {
                if (j + 1 > lastByteBitCount)
                    break;
            }
            if ((nextByte >> j) & 0x01) {
                if (node != NULL && node->right != NULL){
                    node = node->right;
                }
            }
            else {
                if (node != NULL && node->left != NULL)
                    node = node->left;
            }
            if (node != NULL && node->right == NULL && node->left == NULL) {
                unsigned char byte = node->byte;
                // decodedData.push_back(byte);
                decodedData[pos] = byte;
                pos++;
                node = huffmanRoot;
            }
        }
    }
    
    return decodedData;
}

bool checkSuffix3608(const char* filePath) {
    string path = string(filePath);
    string suffix = ".huf";
    if (path.length() >= suffix.length()) {
        return (0 == path.compare (path.length() - suffix.length(), suffix.length(), suffix));
    } else {
        return false;
    }
}

void decode3608(const char* filePath) {
    if (!checkSuffix3608(filePath)) {
        printf("File type not correct. Please input .huf file\n");
        return;
    }
    
    MinHeap3608 minHeap = MinHeap3608();
    vector<unsigned char> encodedData = vector<unsigned char>();
    char lastByteBitCount;
    size_t originFileSize;
    printf("Loading file.\n");
    if (!readEncodedFile3608(filePath, minHeap.minHeap, encodedData, lastByteBitCount, originFileSize)) {
        printf("Read file error.\n");
        return;
    }

    printf("Genarating Huffman code.\n");
    Huffman3608 huffman = Huffman3608();
    huffman.genarateHuffman3608(minHeap);
    
    printf("Decoding.\n");
    vector<unsigned char> decodedData = decode3608(encodedData, huffman.root, lastByteBitCount, originFileSize);
    printf("Encoded size: %ld\nDecoded size: %ld\nWriting file.\n", encodedData.size(), decodedData.size());
    writeDecodedFile3608(filePath, decodedData);
}

int main(int argc, const char *argv[]) {
    printf("\n\tHuffman Coding.\n\tby Min Zeng\n\tDecode\n\n");
    
    string filePath;
    
    if (argc == 2) {
        filePath = argv[1];
    } else {
        printf("Input file path: ");
        cin >> filePath;
    }
    
    decode3608(filePath.c_str());
    
    return 0;
}
