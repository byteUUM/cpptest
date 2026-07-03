#include <deque>
using namespace std;

template<class T, class Container = deque<T>>
class Stack {
public:
    // 判空
    bool empty() const {
        return c.empty();
    }

    // 元素个数
    size_t size() const {
        return c.size();
    }

    // 取栈顶元素
    T& top() {
        return c.back();
    }
    const T& top() const {
        return c.back();
    }

    // 入栈
    void push(const T& val) {
        c.push_back(val);
    }

    // 出栈
    void pop() {
        c.pop_back();
    }

private:
    Container c; // 底层容器，栈的所有操作都转调给它
};
