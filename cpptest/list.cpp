#pragma once
#include <iostream>
#include <stdexcept>

template <typename T>
class MyList {
private:
    struct Node {
        T data;
        Node* next;
        Node(const T& val, Node* n = nullptr) : data(val), next(n) {}
    };

    Node* head_ = nullptr;
    Node* tail_ = nullptr;
    size_t size_ = 0;

puiblic:
    MyList() = default;

    ~MyList() {
        clear();
    }

    // 禁止拷贝，简化实现
    MyList(const MyList&) = delete;
    MyList& operator=(const MyList&) = delete;

    // ---------------- 基本信息 ----------------
    size_t size() const { return size_; }
    bool empty() const { return size_ == 0; }

    void clear() {
        Node* cur = head_;
        while (cur) {
            Node* tmp = cur;
            cur = cur->next;
            delete tmp;
        }
        head_ = tail_ = nullptr;
        size_ = 0;
    }

    // ---------------- 头尾插入 ----------------
    void push_back(const T& val) {
        Node* newNode = new Node(val);
        if (tail_) {
            tail_->next = newNode;
            tail_ = newNode;
        } else {
            head_ = tail_ = newNode;
        }
        ++size_;
    }

    void push_front(const T& val) {
        Node* newNode = new Node(val, head_);
        head_ = newNode;
        if (!tail_) tail_ = newNode;
        ++size_;
    }

    // ---------------- 头尾删除 ----------------
    void pop_front() {
        if (!head_) throw std::out_of_range("MyList::pop_front: list is empty");
        Node* tmp = head_;
        head_ = head_->next;
        if (!head_) tail_ = nullptr;
        delete tmp;
        --size_;
    }

    // 单链表删除尾部是 O(n)，因为要找到倒数第二个节点
    void pop_back() {
        if (!head_) throw std::out_of_range("MyList::pop_back: list is empty");
        if (head_ == tail_) {
            delete head_;
            head_ = tail_ = nullptr;
        } else {
            Node* cur = head_;
            while (cur->next != tail_) cur = cur->next;
            delete tail_;
            tail_ = cur;
            tail_->next = nullptr;
        }
        --size_;
    }

    T& front() {
        if (!head_) throw std::out_of_range("MyList::front: list is empty");
        return head_->data;
    }

    T& back() {
        if (!tail_) throw std::out_of_range("MyList::back: list is empty");
        return tail_->data;
    }

    // ---------------- 查找 ----------------
    bool find(const T& val) const {
        Node* cur = head_;
        while (cur) {
            if (cur->data == val) return true;
            cur = cur->next;
        }
        return false;
    }

    // ---------------- 按值删除（第一个匹配的节点）----------------
    bool erase(const T& val) {
        Node* cur = head_;
        Node* prev = nullptr;
        while (cur) {
            if (cur->data == val) {
                if (prev) prev->next = cur->next;
                else head_ = cur->next;
                if (cur == tail_) tail_ = prev;
                delete cur;
                --size_;
                return true;
            }
            prev = cur;
            cur = cur->next;
        }
        return false;
    }

    // ---------------- 打印（调试用）----------------
    void print() const {
        Node* cur = head_;
        std::cout << "[ ";
        while (cur) {
            std::cout << cur->data << " ";
            cur = cur->next;
        }
        std::cout << "]" << std::endl;
    }

    // ---------------- 迭代器（支持 for 遍历）----------------
    class Iterator {
    public:
        explicit Iterator(Node* node) : node_(node) {}

        T& operator*() { return node_->data; }
        Node* operator->() { return node_; }

        Iterator& operator++() {
            node_ = node_->next;
            return *this;
        }

        bool operator!=(const Iterator& other) const { return node_ != other.node_; }
        bool operator==(const Iterator& other) const { return node_ == other.node_; }

    private:
        Node* node_;
    };

    Iterator begin() { return Iterator(head_); }
    Iterator end() { return Iterator(nullptr); }
};
