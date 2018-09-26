// Min Zeng     cs610 3608 prp

#include "MinHeap3608.h"
#include "Huffman3608.h"
#include <iostream>

MinHeap3608::MinHeap3608() {
    minHeap = vector<Node3608*>();
}

void MinHeap3608::buildMinHeap3608(vector<Node3608*> nodes) {
    minHeap = nodes;
    for (int i = ((int)minHeap.size()-1)/2; i >= 0; i--) {
        downHeap3608(i);
    }
}

void MinHeap3608::downHeap3608(int pos) {
    int i = pos;
    int left = pos*2+1;
    int right = pos*2+2;
    if (left < minHeap.size()) {
        if (minHeap[i]->count > minHeap[left]->count)
            i = left;
    }
    if (right < minHeap.size()) {
        if (minHeap[i]->count > minHeap[right]->count)
            i = right;
    }
    if (i != pos) {
        std::swap(minHeap[i], minHeap[pos]);
        downHeap3608(i);
    }
}

Node3608* MinHeap3608::extractMin3608() {
    Node3608* node = minHeap[0];
    std::swap(minHeap[0], minHeap[minHeap.size()-1]);
    minHeap.pop_back();
    downHeap3608(0);
    return node;
}

void MinHeap3608::showHeap() {
    int a = 1, b = 1;
    for (int i = 0; i < minHeap.size(); i++) {
        if (i == a) {
            std::cout << std::endl;
            b *= 2;
            a += b;
        }
        std::cout << minHeap[i]->count << " ";
    }
}

int MinHeap3608::size3608() {
    return (int)minHeap.size();
}

void MinHeap3608::push3608(Node3608* node) {
    minHeap.push_back(node);
    int current = (int)minHeap.size()-1;
    while (current > 0) {
        int parent = (current-1)/2;
        if (minHeap[current]->count < minHeap[parent]->count) {
            std::swap(minHeap[current], minHeap[parent]);
            current = parent;
        } else {
            return;
        }
    }
}
