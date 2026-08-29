#include <vector>
#include <stdexcept>
using namespace std;

// 最小堆实现的优先级队列（值越小优先级越高）
class PriorityQueue {
private:
    vector<int> data;

    int parent(int i) { return (i - 1) / 2; }
    int left(int i)   { return 2 * i + 1; }
    int right(int i)  { return 2 * i + 2; }

    void siftUp(int i) {
        while (i > 0 && data[parent(i)] > data[i]) {
            swap(data[parent(i)], data[i]);
            i = parent(i);
        }
    }

    void siftDown(int i) {
        int n = data.size();
        while (true) {
            int smallest = i;
            int l = left(i), r = right(i);
            if (l < n && data[l] < data[smallest]) smallest = l;
            if (r < n && data[r] < data[smallest]) smallest = r;
            if (smallest == i) break;
            swap(data[i], data[smallest]);
            i = smallest;
        }
    }

public:
    bool empty() const { return data.empty(); }
    int size() const  { return data.size(); }

    void push(int val) {
        data.push_back(val);
        siftUp(data.size() - 1);
    }

    int top() {
        if (empty()) throw runtime_error("queue is empty");
        return data[0];
    }

    void pop() {
        if (empty()) throw runtime_error("queue is empty");
        data[0] = data.back();
        data.pop_back();
        if (!data.empty()) siftDown(0);
    }
};
