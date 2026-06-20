#include <vector>
#include <stdexcept>
using namespace std;

class MaxHeap {
private:
    vector<int> data;

    // 获取父节点/子节点下标
    int parent(int i) { return (i - 1) / 2; }
    int left(int i)   { return 2 * i + 1; }
    int right(int i)  { return 2 * i + 2; }

    // 上浮：新插入的元素往上调整
    void siftUp(int i) {
        while (i > 0 && data[parent(i)] < data[i]) {
            swap(data[parent(i)], data[i]);
            i = parent(i);
        }
    }

    // 下沉：堆顶元素往下调整
    void siftDown(int i) {
        int n = data.size();
        while (true) {
            int largest = i;
            int l = left(i), r = right(i);
            if (l < n && data[l] > data[largest]) largest = l;
            if (r < n && data[r] > data[largest]) largest = r;
            if (largest == i) break;
            swap(data[i], data[largest]);
            i = largest;
        }
    }

public:
    bool empty() const { return data.empty(); }
    int size() const { return data.size(); }

    // 插入元素：放到末尾，再上浮
    void push(int val) {
        data.push_back(val);
        siftUp(data.size() - 1);
    }

    // 查看堆顶
    int top() {
        if (empty()) throw runtime_error("heap is empty");
        return data[0];
    }

    // 删除堆顶：把最后一个元素移到堆顶，再下沉
    void pop() {
        if (empty()) throw runtime_error("heap is empty");
        data[0] = data.back();
        data.pop_back();
        if (!data.empty()) siftDown(0);
    }

    // O(n) 建堆：从最后一个非叶子节点开始下沉
    void buildHeap(const vector<int>& arr) {
        data = arr;
        for (int i = data.size() / 2 - 1; i >= 0; --i) {
            siftDown(i);
        }
    }
};
