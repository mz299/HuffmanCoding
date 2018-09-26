// Min Zeng     cs610 3608 prp

#include <vector>
using namespace std;

struct Node3608;
class MinHeap3608 {
public:
    vector<Node3608*> minHeap;
    
    void downHeap3608(int pos);
    MinHeap3608();
    void buildMinHeap3608(vector<Node3608*> nodes);
    Node3608* extractMin3608();
    void showHeap();
    int size3608();
    void push3608(Node3608* node);
};
