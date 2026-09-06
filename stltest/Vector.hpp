#ifndef BYTE_VECTOR_HPP
#define BYTE_VECTOR_HPP

#include <cstddef>
#include <cassert>
#include <new>          // placement new
#include <utility>      // std::move / std::forward / std::swap
#include <initializer_list>
#include <type_traits>  // std::enable_if / std::is_integral

namespace byte {

// 简化版 std::vector：底层是一段连续堆内存，
// 用三个指针描述：_start(首元素)、_finish(尾后)、_end_of_storage(容量末尾)。
// 只按需构造元素（placement new + 显式析构），不要求 T 可默认构造。
template <class T>
class vector {
public:
    typedef T* iterator;
    typedef const T* const_iterator;

    // ---------- 构造 / 析构 ----------
    vector() : _start(nullptr), _finish(nullptr), _end_of_storage(nullptr) {}

    // n 个 val
    explicit vector(size_t n, const T& val = T())
        : _start(nullptr), _finish(nullptr), _end_of_storage(nullptr) {
        reserve(n);
        for (size_t i = 0; i < n; ++i) push_back(val);
    }

    // 迭代器区间构造。
    // 用 enable_if 排除整型，避免 vector<int> v(3, 7) 这种"n 个 val"被
    // 误判成"区间 [3, 7)"（这是 STL 实现的经典坑）。
    template <class InputIt,
              class = typename std::enable_if<!std::is_integral<InputIt>::value>::type>
    vector(InputIt first, InputIt last)
        : _start(nullptr), _finish(nullptr), _end_of_storage(nullptr) {
        for (; first != last; ++first) push_back(*first);
    }

    // 初始化列表
    vector(std::initializer_list<T> il)
        : _start(nullptr), _finish(nullptr), _end_of_storage(nullptr) {
        reserve(il.size());
        for (const auto& e : il) push_back(e);
    }

    // 拷贝构造
    vector(const vector& v)
        : _start(nullptr), _finish(nullptr), _end_of_storage(nullptr) {
        reserve(v.size());
        for (const auto& e : v) push_back(e);
    }

    // 移动构造：直接接管三根指针
    vector(vector&& v) noexcept
        : _start(v._start), _finish(v._finish), _end_of_storage(v._end_of_storage) {
        v._start = v._finish = v._end_of_storage = nullptr;
    }

    ~vector() {
        clear();
        ::operator delete(_start);
    }

    // ---------- 赋值 ----------
    vector& operator=(const vector& v) {
        if (this != &v) {
            vector tmp(v); // 拷贝-交换
            swap(tmp);
        }
        return *this;
    }

    vector& operator=(vector&& v) noexcept {
        if (this != &v) {
            clear();
            ::operator delete(_start);
            _start = v._start;
            _finish = v._finish;
            _end_of_storage = v._end_of_storage;
            v._start = v._finish = v._end_of_storage = nullptr;
        }
        return *this;
    }

    void swap(vector& v) noexcept {
        std::swap(_start, v._start);
        std::swap(_finish, v._finish);
        std::swap(_end_of_storage, v._end_of_storage);
    }

    // ---------- 迭代器 ----------
    iterator begin() { return _start; }
    iterator end() { return _finish; }
    const_iterator begin() const { return _start; }
    const_iterator end() const { return _finish; }
    const_iterator cbegin() const { return _start; }
    const_iterator cend() const { return _finish; }

    // ---------- 容量 ----------
    size_t size() const { return _finish - _start; }
    size_t capacity() const { return _end_of_storage - _start; }
    bool empty() const { return _start == _finish; }

    // 预留容量：只分配内存不构造元素；扩容时把旧元素"移动"到新内存
    void reserve(size_t n) {
        if (n <= capacity()) return;
        size_t old_size = size();
        T* tmp = static_cast<T*>(::operator new(n * sizeof(T)));
        // 移动构造旧元素到新内存
        for (size_t i = 0; i < old_size; ++i)
            new (tmp + i) T(std::move(_start[i]));
        // 析构旧元素并释放旧内存
        for (size_t i = 0; i < old_size; ++i)
            _start[i].~T();
        ::operator delete(_start);

        _start = tmp;
        _finish = tmp + old_size;
        _end_of_storage = tmp + n;
    }

    // 调整大小：变长补 val，变短析构多余元素
    void resize(size_t n, const T& val = T()) {
        if (n < size()) {
            while (_finish != _start + n) {
                --_finish;
                _finish->~T();
            }
        } else if (n > size()) {
            if (n > capacity()) reserve(n);
            while (_finish != _start + n) {
                new (_finish) T(val);
                ++_finish;
            }
        }
    }

    // ---------- 元素访问 ----------
    T& operator[](size_t i) { return _start[i]; }
    const T& operator[](size_t i) const { return _start[i]; }

    T& at(size_t i) { assert(i < size()); return _start[i]; }
    const T& at(size_t i) const { assert(i < size()); return _start[i]; }

    T& front() { return *_start; }
    const T& front() const { return *_start; }
    T& back() { return *(_finish - 1); }
    const T& back() const { return *(_finish - 1); }

    T* data() { return _start; }
    const T* data() const { return _start; }

    // ---------- 修改 ----------
    void push_back(const T& val) {
        if (_finish == _end_of_storage)
            reserve(capacity() == 0 ? 1 : 2 * capacity()); // 2 倍扩容
        new (_finish) T(val);
        ++_finish;
    }

    void push_back(T&& val) {
        if (_finish == _end_of_storage)
            reserve(capacity() == 0 ? 1 : 2 * capacity());
        new (_finish) T(std::move(val));
        ++_finish;
    }

    // 原地构造，省一次拷贝/移动
    template <class... Args>
    void emplace_back(Args&&... args) {
        if (_finish == _end_of_storage)
            reserve(capacity() == 0 ? 1 : 2 * capacity());
        new (_finish) T(std::forward<Args>(args)...);
        ++_finish;
    }

    void pop_back() {
        if (_finish != _start) {
            --_finish;
            _finish->~T();
        }
    }

    // 在 pos 处插入一个元素
    iterator insert(const_iterator pos, const T& val) {
        size_t off = pos - _start; // 记录偏移，防止扩容后 pos 失效
        if (_finish == _end_of_storage)
            reserve(capacity() == 0 ? 1 : 2 * capacity());
        iterator p = _start + off;
        if (_finish != _start) {
            new (_finish) T(std::move(*(_finish - 1))); // 把最后一个元素移到最后
            ++_finish;
            for (iterator it = _finish - 2; it != p; --it) // 从后往前搬
                *it = std::move(*(it - 1));
            *p = val;
        } else {
            new (_finish) T(val);
            ++_finish;
        }
        return p;
    }

    // 删除 pos 处的元素
    iterator erase(const_iterator pos) {
        size_t off = pos - _start;
        for (iterator it = _start + off; it != _finish - 1; ++it) // 从前往后搬
            *it = std::move(*(it + 1));
        --_finish;
        _finish->~T();
        return _start + off;
    }

    // 删除 [first, last) 区间
    iterator erase(const_iterator first, const_iterator last) {
        size_t off = first - _start;
        iterator f = _start + off;
        size_t n = last - first;
        for (iterator it = f; it != _finish - n; ++it)
            *it = std::move(*(it + n));
        while (n-- > 0) pop_back();
        return f;
    }

    void clear() {
        while (_finish != _start) pop_back();
    }

private:
    T* _start;           // 首元素
    T* _finish;          // 尾后
    T* _end_of_storage;  // 容量末尾
};

// ---------- 自由函数：比较 ----------
template <class T>
bool operator==(const vector<T>& a, const vector<T>& b) {
    if (a.size() != b.size()) return false;
    for (size_t i = 0; i < a.size(); ++i)
        if (!(a[i] == b[i])) return false;
    return true;
}
template <class T>
bool operator!=(const vector<T>& a, const vector<T>& b) { return !(a == b); }

template <class T>
bool operator<(const vector<T>& a, const vector<T>& b) {
    size_t n = a.size() < b.size() ? a.size() : b.size();
    for (size_t i = 0; i < n; ++i) {
        if (a[i] < b[i]) return true;
        if (b[i] < a[i]) return false;
    }
    return a.size() < b.size();
}
template <class T>
bool operator>(const vector<T>& a, const vector<T>& b) { return b < a; }
template <class T>
bool operator<=(const vector<T>& a, const vector<T>& b) { return !(b < a); }
template <class T>
bool operator>=(const vector<T>& a, const vector<T>& b) { return !(a < b); }

} // namespace byte

#endif // BYTE_VECTOR_HPP
