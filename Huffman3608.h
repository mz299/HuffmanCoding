// Min Zeng     cs610 3608 prp

#include <vector>
#include <iostream>
using namespace std;

struct Node3608 {
    unsigned char byte;
    int count;
    Node3608* left;
    Node3608* right;
    Node3608() {
        byte = 0;
        count = 0;
        left = NULL;
        right = NULL;
    }
    Node3608(unsigned char b, int c) {
        byte = b;
        count = c;
        left = NULL;
        right = NULL;
    }
    Node3608(Node3608* node1, Node3608* node2) {
        byte = 0;
        count = node1->count + node2->count;
        left = node1;
        right = node2;
    }
    ~Node3608() {
        if (left) delete left;
        if (right) delete right;
    }
};

class MinHeap3608;
class Huffman3608 {
public:
    Node3608* root;
    string codebook[256];
    
    Huffman3608();
    ~Huffman3608();
    void genarateHuffman3608(MinHeap3608 minHeap);
    void genarateCodebook3608();
    void fillCodebook3608(Node3608* node, string& code);
};
