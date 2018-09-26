// Min Zeng     cs610 3608 prp

#include <iostream>
#include <fstream>
#include <vector>
#include "Huffman3608.h"
#include "MinHeap3608.h"

using namespace std;

bool readOriginFile3608(const char* filePath, vector<unsigned char> &filedata) {
    ifstream fin(filePath, ios::binary);
    if (fin.fail()) {
        return false;
    }

    streampos fsize = fin.tellg();
    fin.seekg(0, ios::end);
    fsize = fin.tellg() - fsize;
    fin.clear();
    fin.seekg(0, ios::beg);

    filedata.resize(fsize);

    fin.read(reinterpret_cast<char*>(&filedata[0]), fsize);

    fin.close();
    return true;
}

void writeEncodedFile3608(const char* filePath, vector<Node3608*> minHeap, vector<unsigned char> encodedData, char lastByteBitCount, size_t originFileSize) {
    string encodedFilePath = string(filePath);
    encodedFilePath += ".huf";
    
    size_t size = minHeap.size();
    
    ofstream fout(encodedFilePath.c_str(), ios::out | ios::binary);
    fout.write(reinterpret_cast<char*>(&size), sizeof(size));

    unsigned char bytes[256];
    int counts[256];
    for (int i = 0; i < size; i++) {
        bytes[i] = minHeap[i]->byte;
        counts[i] = minHeap[i]->count;
    }
    fout.write(reinterpret_cast<char*>(&bytes[0]), size * sizeof(unsigned char));
    fout.write(reinterpret_cast<char*>(&counts[0]), size * sizeof(int));
    fout.write(reinterpret_cast<char*>(&originFileSize), sizeof(originFileSize));
    fout.write(reinterpret_cast<char*>(&lastByteBitCount), sizeof(lastByteBitCount));
    fout.write(reinterpret_cast<char*>(&encodedData[0]), encodedData.size() * sizeof(unsigned char));
    fout.close();
    
    printf("Output file: %s\n", encodedFilePath.c_str());
}

vector<Node3608*> filedataToNodes3608(vector<unsigned char> filedata) {
    vector<Node3608*> nodes = vector<Node3608*>();
    int table[256] = {0};
    for (int i = 0; i < filedata.size(); i++) {
        table[filedata[i]]++;
    }
    for (int i = 0; i < 256; i++) {
        if (table[i] == 0)
            continue;
        Node3608* node = new Node3608(i, table[i]);
        nodes.push_back(node);
    }
    return nodes;
}

vector<unsigned char> encode3608(vector<unsigned char> filedata, const string codebook[], char &lastByteBitCount) {
    vector<unsigned char> encodedData = vector<unsigned char>();
    encodedData.resize(filedata.size());

    unsigned char nextByte = 0;
    char bitCounter = 0;
    size_t size = 0;
    for (int i = 0; i < filedata.size(); i++) {
        unsigned char nextChar = filedata[i];
        for (int i = 0; i < codebook[nextChar].size(); i++, bitCounter++) {
            if (bitCounter == 8) {
                if (encodedData.size() > size) {
                    encodedData[size] = nextByte;
                } else {
                    encodedData.push_back(nextByte);
                }
                size++;
                nextByte = 0;
                bitCounter = 0;
            }
            if (codebook[nextChar][i] == '1') {
                nextByte = nextByte | (0x01 << bitCounter);
            }
        }
    }
    if (bitCounter) {
        if (encodedData.size() > size) {
            encodedData[size] = nextByte;
        }
        else {
            encodedData.push_back(nextByte);
        }
        size++;
    }
    encodedData.resize(size);
    
    lastByteBitCount = bitCounter;
    return encodedData;
}

void encode3608(const char* filePath) {
    vector<unsigned char> filedata = vector<unsigned char>();
    printf("Loading file.\n");
    if (!readOriginFile3608(filePath, filedata)) {
        printf("Read file error.\n");
        return;
    }
    
    printf("Genarating Huffman code.\n");
    vector<Node3608*> nodes = filedataToNodes3608(filedata);
    
    MinHeap3608 minHeap = MinHeap3608();
    minHeap.buildMinHeap3608(nodes);
    
    Huffman3608 huffman = Huffman3608();
    huffman.genarateHuffman3608(minHeap);
    huffman.genarateCodebook3608();
    
    printf("Encoding.\n");
    char lastByteBitCount;
    vector<unsigned char> encodedData = encode3608(filedata, huffman.codebook, lastByteBitCount);
    printf("Original size: %ld\nEncoded size: %ld\nWriting file.\n", filedata.size(), encodedData.size());
    writeEncodedFile3608(filePath, minHeap.minHeap, encodedData, lastByteBitCount, filedata.size());
}

int main(int argc, const char *argv[]) {
    printf("\n\tHuffman Coding.\n\tby Min Zeng\n\tEncode\n\n");

    string filePath;
    
    if (argc == 2) {
        filePath = argv[1];
    } else {
        printf("Input file path: ");
        cin >> filePath;
    }
    
    encode3608(filePath.c_str());
    
    return 0;
}
