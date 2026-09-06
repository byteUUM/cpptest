#ifndef BYTE_STRING_HPP
#define BYTE_STRING_HPP

#include <cstring>   // strlen / memcpy / memcmp
#include <cassert>
#include <utility>   // std::move / std::swap
#include <algorithm> // std::min
#include <iostream>  // ostream / istream

namespace byte {

// 简化版 std::string：底层是一个以 '\0' 结尾的 char 数组，
// 三个成员：_str(指向堆内存)、_size(有效字符数)、_capacity(已分配容量)。
// 遵循"三/五法则"：拷贝构造、拷贝赋值、移动构造、移动赋值、析构。
class string {
public:
    typedef char* iterator;
    typedef const char* const_iterator;

    // 未找到时 find 返回的值
    static const size_t npos = static_cast<size_t>(-1);

    // ---------- 构造 / 析构 ----------
    // 默认构造 / 从 C 字符串构造
    string(const char* str = "")
        : _str(nullptr), _size(0), _capacity(0) {
        _size = std::strlen(str);
        _capacity = _size;
        _str = new char[_capacity + 1];
        std::memcpy(_str, str, _size + 1);
    }

    // n 个字符 c
    string(size_t n, char c)
        : _str(nullptr), _size(n), _capacity(n) {
        _str = new char[n + 1];
        for (size_t i = 0; i < n; ++i) _str[i] = c;
        _str[n] = '\0';
    }

    // 从 str 拷贝前 n 个字符（供 substr 等内部使用）
    string(const char* str, size_t n)
        : _str(nullptr), _size(n), _capacity(n) {
        _str = new char[n + 1];
        std::memcpy(_str, str, n);
        _str[n] = '\0';
    }

    // 拷贝构造
    string(const string& s)
        : _str(nullptr), _size(0), _capacity(0) {
        reserve(s._size);
        append(s._str, s._size);
    }

    // 移动构造：直接"接管"资源，源对象置空
    string(string&& s) noexcept
        : _str(s._str), _size(s._size), _capacity(s._capacity) {
        s._str = nullptr;
        s._size = s._capacity = 0;
    }

    ~string() {
        delete[] _str;
        _str = nullptr;
        _size = _capacity = 0;
    }

    // ---------- 赋值 ----------
    string& operator=(const string& s) {
        if (this != &s) {
            string tmp(s); // 拷贝-交换惯用法，强异常安全
            swap(tmp);
        }
        return *this;
    }

    string& operator=(string&& s) noexcept {
        if (this != &s) {
            delete[] _str;
            _str = s._str;
            _size = s._size;
            _capacity = s._capacity;
            s._str = nullptr;
            s._size = s._capacity = 0;
        }
        return *this;
    }

    string& operator=(const char* str) {
        *this = string(str);
        return *this;
    }

    void swap(string& s) noexcept {
        std::swap(_str, s._str);
        std::swap(_size, s._size);
        std::swap(_capacity, s._capacity);
    }

    // ---------- 迭代器 ----------
    iterator begin() { return _str; }
    iterator end() { return _str + _size; }
    const_iterator begin() const { return _str; }
    const_iterator end() const { return _str + _size; }
    const_iterator cbegin() const { return _str; }
    const_iterator cend() const { return _str + _size; }

    // ---------- 容量 ----------
    size_t size() const { return _size; }
    size_t length() const { return _size; }
    size_t capacity() const { return _capacity; }
    bool empty() const { return _size == 0; }

    // 预留空间：只在需要更大容量时才重新分配
    void reserve(size_t n) {
        if (n > _capacity) {
            char* tmp = new char[n + 1];
            if (_str) {
                std::memcpy(tmp, _str, _size + 1);
                delete[] _str;
            } else {
                tmp[0] = '\0';
            }
            _str = tmp;
            _capacity = n;
        }
    }

    // 调整有效长度：变长用 c 填充，变短则截断
    void resize(size_t n, char c = '\0') {
        if (n < _size) {
            _size = n;
            _str[n] = '\0';
        } else if (n > _size) {
            reserve(n);
            for (size_t i = _size; i < n; ++i) _str[i] = c;
            _size = n;
            _str[n] = '\0';
        }
    }

    void clear() {
        _size = 0;
        _str[0] = '\0';
    }

    // ---------- 元素访问 ----------
    char& operator[](size_t i) { return _str[i]; }
    const char& operator[](size_t i) const { return _str[i]; }

    char& at(size_t i) { assert(i < _size); return _str[i]; }
    const char& at(size_t i) const { assert(i < _size); return _str[i]; }

    char& front() { return _str[0]; }
    const char& front() const { return _str[0]; }
    char& back() { return _str[_size - 1]; }
    const char& back() const { return _str[_size - 1]; }

    // 返回 C 风格字符串，保证以 '\0' 结尾
    const char* c_str() const { return _str; }
    const char* data() const { return _str; }

    // ---------- 修改 ----------
    void push_back(char c) {
        if (_size == _capacity)
            reserve(_capacity == 0 ? 8 : 2 * _capacity); // 2 倍扩容
        _str[_size++] = c;
        _str[_size] = '\0';
    }

    void append(const char* str) {
        append(str, std::strlen(str));
    }

    void append(const char* str, size_t n) {
        if (_size + n > _capacity) {
            size_t new_cap = _size + n > _capacity * 2 ? _size + n : _capacity * 2;
            reserve(new_cap); // 一次开够，减少反复扩容
        }
        std::memcpy(_str + _size, str, n);
        _size += n;
        _str[_size] = '\0';
    }

    string& operator+=(char c) { push_back(c); return *this; }
    string& operator+=(const char* str) { append(str); return *this; }
    string& operator+=(const string& s) { append(s._str, s._size); return *this; }

    // 在 pos 处插入一个字符
    string& insert(size_t pos, char c) {
        assert(pos <= _size);
        if (_size == _capacity)
            reserve(_capacity == 0 ? 8 : 2 * _capacity);
        for (size_t i = _size; i > pos; --i)  // 从后往前搬
            _str[i] = _str[i - 1];
        _str[pos] = c;
        ++_size;
        _str[_size] = '\0';
        return *this;
    }

    // 从 pos 起删除 len 个字符（len 默认删到末尾）
    string& erase(size_t pos, size_t len = npos) {
        assert(pos <= _size);
        if (len == npos || pos + len >= _size) {
            _size = pos;
        } else {
            for (size_t i = pos; i + len <= _size; ++i) // 从前往后搬
                _str[i] = _str[i + len];
            _size -= len;
        }
        _str[_size] = '\0';
        return *this;
    }

    void pop_back() {
        if (_size > 0) _str[--_size] = '\0';
    }

    // ---------- 查找 ----------
    size_t find(char c, size_t pos = 0) const {
        for (size_t i = pos; i < _size; ++i)
            if (_str[i] == c) return i;
        return npos;
    }

    size_t find(const char* str, size_t pos = 0) const {
        size_t len = std::strlen(str);
        if (len == 0) return pos <= _size ? pos : npos;
        for (size_t i = pos; i + len <= _size; ++i)
            if (std::memcmp(_str + i, str, len) == 0) return i;
        return npos;
    }

    // 子串
    string substr(size_t pos = 0, size_t len = npos) const {
        assert(pos <= _size);
        size_t n = (len == npos || pos + len > _size) ? _size - pos : len;
        return string(_str + pos, n);
    }

private:
    char* _str;
    size_t _size;
    size_t _capacity;
};

// ---------- 自由函数：输入输出 ----------
inline std::ostream& operator<<(std::ostream& out, const string& s) {
    for (size_t i = 0; i < s.size(); ++i) out << s[i];
    return out;
}

inline std::istream& operator>>(std::istream& in, string& s) {
    s.clear();
    char c;
    while (in.get(c) && (c == ' ' || c == '\n' || c == '\t')) {} // 跳过前导空白
    if (!in) return in;
    do {
        s.push_back(c);
    } while (in.get(c) && c != ' ' && c != '\n' && c != '\t');
    return in;
}

// ---------- 自由函数：比较 ----------
inline bool operator==(const string& a, const string& b) {
    return a.size() == b.size() && std::memcmp(a.data(), b.data(), a.size()) == 0;
}
inline bool operator!=(const string& a, const string& b) { return !(a == b); }

inline bool operator<(const string& a, const string& b) {
    size_t n = std::min(a.size(), b.size());
    int cmp = (n == 0) ? 0 : std::memcmp(a.data(), b.data(), n);
    if (cmp != 0) return cmp < 0;
    return a.size() < b.size(); // 前缀相同时，短的小
}
inline bool operator>(const string& a, const string& b) { return b < a; }
inline bool operator<=(const string& a, const string& b) { return !(b < a); }
inline bool operator>=(const string& a, const string& b) { return !(a < b); }

// string 与 C 字符串混合比较
inline bool operator==(const string& a, const char* b) { return a == string(b); }
inline bool operator==(const char* a, const string& b) { return string(a) == b; }
inline bool operator!=(const string& a, const char* b) { return !(a == b); }
inline bool operator!=(const char* a, const string& b) { return !(a == b); }

// ---------- 自由函数：拼接 ----------
inline string operator+(const string& a, const string& b) {
    string r;
    r.reserve(a.size() + b.size());
    r += a;
    r += b;
    return r;
}
inline string operator+(const string& a, const char* b) { return a + string(b); }
inline string operator+(const char* a, const string& b) { return string(a) + b; }
inline string operator+(const string& a, char b) { return a + string(1, b); }
inline string operator+(char a, const string& b) { return string(1, a) + b; }

} // namespace byte

#endif // BYTE_STRING_HPP
