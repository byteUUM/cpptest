#pragma once
#include <cassert>
#include <cstring>
#include <iostream>
#include <stdexcept>

namespace mystd {

class string {
public:
    // ==================== 类型别名 ====================
    using size_type = size_t;
    static const size_type npos = static_cast<size_type>(-1);

    // ==================== 构造 / 析构 ====================

    // 默认构造
    string() : _data(new char[1]{'\0'}), _size(0), _capacity(0) {}

    // C字符串构造
    string(const char* str) {
        assert(str != nullptr);
        _size = strlen(str);
        _capacity = _size;
        _data = new char[_capacity + 1];
        memcpy(_data, str, _size + 1);
    }

    // 填充构造：n 个字符 c
    string(size_type n, char c) {
        _size = n;
        _capacity = n;
        _data = new char[_capacity + 1];
        memset(_data, c, n);
        _data[n] = '\0';
    }

    // 拷贝构造
    string(const string& other) {
        _size = other._size;
        _capacity = other._size;
        _data = new char[_capacity + 1];
        memcpy(_data, other._data, _size + 1);
    }

    // 移动构造
    string(string&& other) noexcept
        : _data(other._data), _size(other._size), _capacity(other._capacity) {
        other._data = new char[1]{'\0'};
        other._size = 0;
        other._capacity = 0;
    }

    ~string() { delete[] _data; }

    // ==================== 赋值运算符 ====================

    string& operator=(const string& other) {
        if (this != &other) {
            string tmp(other);
            swap(tmp);
        }
        return *this;
    }

    string& operator=(const char* str) {
        string tmp(str);
        swap(tmp);
        return *this;
    }

    string& operator=(string&& other) noexcept {
        swap(other);
        return *this;
    }

    // ==================== 迭代器 ====================

    char*       begin()       { return _data; }
    const char* begin() const { return _data; }
    char*       end()         { return _data + _size; }
    const char* end()   const { return _data + _size; }

    // ==================== 容量 ====================

    size_type size()     const { return _size; }
    size_type length()   const { return _size; }
    size_type capacity() const { return _capacity; }
    bool      empty()    const { return _size == 0; }

    void reserve(size_type new_cap) {
        if (new_cap <= _capacity) return;
        char* new_data = new char[new_cap + 1];
        memcpy(new_data, _data, _size + 1);
        delete[] _data;
        _data = new_data;
        _capacity = new_cap;
    }

    void resize(size_type new_size, char fill = '\0') {
        if (new_size > _capacity) reserve(new_size);
        if (new_size > _size)
            memset(_data + _size, fill, new_size - _size);
        _size = new_size;
        _data[_size] = '\0';
    }

    void clear() {
        _size = 0;
        _data[0] = '\0';
    }

    // ==================== 元素访问 ====================

    char& operator[](size_type pos)       { return _data[pos]; }
    char  operator[](size_type pos) const { return _data[pos]; }

    char& at(size_type pos) {
        if (pos >= _size) throw std::out_of_range("string::at out of range");
        return _data[pos];
    }
    char at(size_type pos) const {
        if (pos >= _size) throw std::out_of_range("string::at out of range");
        return _data[pos];
    }

    char& front()       { return _data[0]; }
    char  front() const { return _data[0]; }
    char& back()        { return _data[_size - 1]; }
    char  back()  const { return _data[_size - 1]; }

    const char* c_str() const { return _data; }
    const char* data()  const { return _data; }

    // ==================== 修改器 ====================

    void push_back(char c) {
        if (_size == _capacity) _grow();
        _data[_size++] = c;
        _data[_size] = '\0';
    }

    void pop_back() {
        if (_size > 0) _data[--_size] = '\0';
    }

    string& append(const string& other) {
        return append(other._data, other._size);
    }

    string& append(const char* str) {
        return append(str, strlen(str));
    }

    string& append(const char* str, size_type len) {
        if (_size + len > _capacity) reserve(_size + len);
        memcpy(_data + _size, str, len);
        _size += len;
        _data[_size] = '\0';
        return *this;
    }

    string& operator+=(const string& other) { return append(other); }
    string& operator+=(const char* str)     { return append(str); }
    string& operator+=(char c) { push_back(c); return *this; }

    // insert：在 pos 处插入 str
    string& insert(size_type pos, const char* str) {
        size_type len = strlen(str);
        if (pos > _size) throw std::out_of_range("string::insert out of range");
        if (_size + len > _capacity) reserve(_size + len);
        // 把 [pos, _size] 往后挪
        memmove(_data + pos + len, _data + pos, _size - pos + 1);
        memcpy(_data + pos, str, len);
        _size += len;
        return *this;
    }

    string& insert(size_type pos, const string& other) {
        return insert(pos, other._data);
    }

    // erase：从 pos 开始删除 len 个字符
    string& erase(size_type pos = 0, size_type len = npos) {
        if (pos > _size) throw std::out_of_range("string::erase out of range");
        if (len == npos || pos + len >= _size) {
            _size = pos;
        } else {
            memmove(_data + pos, _data + pos + len, _size - pos - len + 1);
            _size -= len;
        }
        _data[_size] = '\0';
        return *this;
    }

    // replace：将 [pos, pos+len) 替换为 str
    string& replace(size_type pos, size_type len, const string& str) {
        erase(pos, len);
        insert(pos, str._data);
        return *this;
    }

    void swap(string& other) noexcept {
        std::swap(_data,     other._data);
        std::swap(_size,     other._size);
        std::swap(_capacity, other._capacity);
    }

    // ==================== 查找 ====================

    // 从 from 开始正向查找子串，返回起始下标，失败返回 npos
    size_type find(const char* pattern, size_type from = 0) const {
        if (from > _size) return npos;
        const char* p = strstr(_data + from, pattern);
        return p ? static_cast<size_type>(p - _data) : npos;
    }

    size_type find(const string& other, size_type from = 0) const {
        return find(other._data, from);
    }

    size_type find(char c, size_type from = 0) const {
        for (size_type i = from; i < _size; ++i)
            if (_data[i] == c) return i;
        return npos;
    }

    // 从 from 开始反向查找
    size_type rfind(char c, size_type from = npos) const {
        if (_size == 0) return npos;
        size_type i = (from == npos || from >= _size) ? _size - 1 : from;
        while (true) {
            if (_data[i] == c) return i;
            if (i == 0) break;
            --i;
        }
        return npos;
    }

    // ==================== 子串 ====================

    string substr(size_type pos = 0, size_type len = npos) const {
        if (pos > _size) throw std::out_of_range("string::substr out of range");
        size_type actual_len = std::min(len, _size - pos);
        string result;
        result.reserve(actual_len);
        memcpy(result._data, _data + pos, actual_len);
        result._data[actual_len] = '\0';
        result._size = actual_len;
        return result;
    }

    // ==================== 比较 ====================

    int compare(const string& other) const {
        return strcmp(_data, other._data);
    }

    // ==================== 关系运算符 ====================

    friend bool operator==(const string& a, const string& b) { return strcmp(a._data, b._data) == 0; }
    friend bool operator!=(const string& a, const string& b) { return !(a == b); }
    friend bool operator< (const string& a, const string& b) { return strcmp(a._data, b._data) < 0; }
    friend bool operator> (const string& a, const string& b) { return b < a; }
    friend bool operator<=(const string& a, const string& b) { return !(a > b); }
    friend bool operator>=(const string& a, const string& b) { return !(a < b); }

    // ==================== 拼接运算符 ====================

    friend string operator+(string lhs, const string& rhs) { lhs += rhs; return lhs; }
    friend string operator+(string lhs, const char*   rhs) { lhs += rhs; return lhs; }
    friend string operator+(const char* lhs, const string& rhs) { return string(lhs) + rhs; }

    // ==================== 流运算符 ====================

    friend std::ostream& operator<<(std::ostream& os, const string& s) {
        return os << s._data;
    }

    friend std::istream& operator>>(std::istream& is, string& s) {
        s.clear();
        char c;
        while (is.get(c) && c != ' ' && c != '\n' && c != '\t')
            s.push_back(c);
        return is;
    }

    // getline 支持
    friend std::istream& getline(std::istream& is, string& s, char delim = '\n') {
        s.clear();
        char c;
        while (is.get(c) && c != delim)
            s.push_back(c);
        return is;
    }

private:
    char*     _data;
    size_type _size;
    size_type _capacity;

    // 扩容策略：翻倍（至少保证 capacity >= 1）
    void _grow() {
        reserve(_capacity == 0 ? 4 : _capacity * 2);
    }
};

} // namespace mystd
