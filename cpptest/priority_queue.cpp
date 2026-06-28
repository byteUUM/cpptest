/**
 * 优先级队列（大根堆）模拟实现
 *
 * 核心思路：用完全二叉树（数组存储）维护堆性质
 *   - 父节点下标 i，左子 2i+1，右子 2i+2
 *   - push：尾插 + 向上调整 (sift_up)
 *   - pop ：交换堆顶与末尾，删尾 + 向下调整 (sift_down)
 *   - top ：直接返回 heap[0]
 *
 * 支持自定义比较器，默认大根堆（等价于 std::priority_queue<T>）
 */

#include <iostream>
#include <vector>
#include <stdexcept>
#include <functional>

template <typename T, typename Compare = std::less<T>>
class PriorityQueue {
public:
    PriorityQueue() = default;

    // 从已有序列批量建堆，O(n)
    explicit PriorityQueue(const std::vector<T>& data) : heap_(data) {
        build_heap();
    }

    // 插入元素，O(log n)
    void push(const T& val) {
        heap_.push_back(val);
        sift_up(heap_.size() - 1);
    }

    // 移除堆顶，O(log n)
    void pop() {
        if (empty()) throw std::underflow_error("priority_queue is empty");
        std::swap(heap_[0], heap_.back());
        heap_.pop_back();
        if (!empty()) sift_down(0);
    }

    // 查看堆顶，O(1)
    const T& top() const {
        if (empty()) throw std::underflow_error("priority_queue is empty");
        return heap_[0];
    }

    bool  empty() const { return heap_.empty(); }
    size_t size() const { return heap_.size(); }

private:
    std::vector<T> heap_;
    Compare        cmp_;   // 默认 less<T>，配合堆逻辑实现大根堆

    /* --------- 索引工具 --------- */
    static size_t parent(size_t i) { return (i - 1) / 2; }
    static size_t left  (size_t i) { return 2 * i + 1; }
    static size_t right (size_t i) { return 2 * i + 2; }

    /* --------- 向上调整（插入后修复） --------- */
    void sift_up(size_t i) {
        while (i > 0) {
            size_t p = parent(i);
            // cmp_(heap_[p], heap_[i]) 即 heap_[p] < heap_[i]，子比父大则上浮
            if (cmp_(heap_[p], heap_[i])) {
                std::swap(heap_[p], heap_[i]);
                i = p;
            } else {
                break;
            }
        }
    }

    /* --------- 向下调整（删除堆顶后修复） --------- */
    void sift_down(size_t i) {
        size_t n = heap_.size();
        while (true) {
            size_t largest = i;
            size_t l = left(i), r = right(i);

            if (l < n && cmp_(heap_[largest], heap_[l]))  largest = l;
            if (r < n && cmp_(heap_[largest], heap_[r]))  largest = r;

            if (largest == i) break;   // 已满足堆性质
            std::swap(heap_[i], heap_[largest]);
            i = largest;
        }
    }

    /* --------- 批量建堆，从最后一个非叶子节点向上 sift_down --------- */
    void build_heap() {
        if (heap_.size() <= 1) return;
        // 最后一个非叶子节点下标
        for (int i = (int)heap_.size() / 2 - 1; i >= 0; --i)
            sift_down(i);
    }
};

/* ===================== 测试 ===================== */
int main() {
    std::cout << "===== 默认大根堆 =====\n";
    PriorityQueue<int> pq;
    for (int x : {3, 1, 4, 1, 5, 9, 2, 6, 5}) {
        pq.push(x);
        std::cout << "push(" << x << ")  top=" << pq.top() << "\n";
    }

    std::cout << "\n出堆顺序（降序）：";
    while (!pq.empty()) {
        std::cout << pq.top() << " ";
        pq.pop();
    }
    std::cout << "\n";

    /* ----- 小根堆：传入 greater<int> ----- */
    std::cout << "\n===== 小根堆（greater<int>）=====\n";
    PriorityQueue<int, std::greater<int>> min_pq({5, 3, 8, 1, 7});
    std::cout << "出堆顺序（升序）：";
    while (!min_pq.empty()) {
        std::cout << min_pq.top() << " ";
        min_pq.pop();
    }
    std::cout << "\n";

    /* ----- 批量建堆 O(n) ----- */
    std::cout << "\n===== 批量建堆（从 vector 构造）=====\n";
    PriorityQueue<int> pq2({10, 20, 5, 30, 15});
    std::cout << "堆顶（最大值）= " << pq2.top() << "\n";   // 应为 30

    /* ----- 异常测试 ----- */
    std::cout << "\n===== 空队列异常测试 =====\n";
    PriorityQueue<int> empty_pq;
    try {
        empty_pq.top();
    } catch (const std::underflow_error& e) {
        std::cout << "捕获异常: " << e.what() << "\n";
    }

    return 0;
}
