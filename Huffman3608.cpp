// Min Zeng     cs610 3608 prp

#include "Huffman3608.h"
#include "MinHeap3608.h"

Huffman3608::Huffman3608() {
    root = NULL;
}

Huffman3608::~Huffman3608() {
    if (root) delete root;
}

void Huffman3608::genarateHuffman3608(MinHeap3608 minHeap) {
    Node3608* node1;
    Node3608* node2;
    Node3608* merge;
    while (minHeap.size3608() > 1) {
        node1 = minHeap.extractMin3608();
        node2 = minHeap.extractMin3608();
        merge = new Node3608(node1, node2);
        minHeap.push3608(merge);
    }
    
    root = minHeap.extractMin3608();
}

void Huffman3608::genarateCodebook3608() {
    string code;
    fillCodebook3608(root, code);
}

void Huffman3608::fillCodebook3608(Node3608* node, string& code) {
    if (!node) {
        return;
    }
    if (!node->left && !node->right) {
        codebook[node->byte] = code;
        // printf("Byte: %c \tCount: %d \tCode: %s\n", node->byte, node->count,code.c_str());
        return;
    }
    if (node->left) {
        code += '0';
        fillCodebook3608(node->left, code);
        code.erase(code.end() - 1);
    }
    if (node->right) {
        code += '1';
        fillCodebook3608(node->right, code);
        code.erase(code.end() - 1);
    }
}
