#include <iostream>

// 控制块：强引用 + 弱引用计数
struct ControlBlock {
    int strong = 0;
    int weak   = 0;
};

// -------- shared_ptr --------
template <typename T>
class shared_ptr {
    template <typename U> friend class weak_ptr;
    T*           ptr_;
    ControlBlock* cb_;

    void release() {
        if (!cb_) return;
        if (--cb_->strong == 0) {
            delete ptr_;
            ptr_ = nullptr;
            if (cb_->weak == 0) { delete cb_; }
            cb_ = nullptr;
        }
    }
public:
    explicit shared_ptr(T* p = nullptr)
        : ptr_(p), cb_(p ? new ControlBlock{1, 0} : nullptr) {}

    shared_ptr(const shared_ptr& o) : ptr_(o.ptr_), cb_(o.cb_)
        { if (cb_) ++cb_->strong; }

    shared_ptr(shared_ptr&& o) noexcept : ptr_(o.ptr_), cb_(o.cb_)
        { o.ptr_ = nullptr; o.cb_ = nullptr; }

    shared_ptr& operator=(shared_ptr o) noexcept  // copy-and-swap
        { std::swap(ptr_, o.ptr_); std::swap(cb_, o.cb_); return *this; }

    ~shared_ptr() { release(); }

    T* get()        const { return ptr_; }
    T& operator*()  const { return *ptr_; }
    T* operator->() const { return ptr_; }
    int use_count() const { return cb_ ? cb_->strong : 0; }
    explicit operator bool() const { return ptr_; }
};

// -------- weak_ptr --------
template <typename T>
class weak_ptr {
    T*            ptr_;
    ControlBlock* cb_;

    void release()
        { if (cb_ && --cb_->weak == 0 && cb_->strong == 0) delete cb_; }
public:
    weak_ptr() : ptr_(nullptr), cb_(nullptr) {}

    weak_ptr(const shared_ptr<T>& sp) : ptr_(sp.ptr_), cb_(sp.cb_)
        { if (cb_) ++cb_->weak; }

    ~weak_ptr() { release(); }

    bool expired() const { return !cb_ || cb_->strong == 0; }

    shared_ptr<T> lock() const {
        if (expired()) return shared_ptr<T>();
        ++cb_->strong;                    // 先抢占引用计数
        shared_ptr<T> sp;
        sp.ptr_ = ptr_; sp.cb_ = cb_;    // 绕过构造函数直接赋值
        return sp;
    }
};

// -------- 测试 --------
// int main() {
//     auto* raw = new int(42);
//     shared_ptr<int> sp1(raw);
//     shared_ptr<int> sp2 = sp1;
//     std::cout << *sp1 << " count=" << sp1.use_count() << "\n"; // 42 count=2

//     weak_ptr<int> wp(sp1);
//     std::cout << "expired=" << wp.expired() << "\n";           // 0

//     sp1 = shared_ptr<int>(); sp2 = shared_ptr<int>();          // 释放强引用
//     std::cout << "expired=" << wp.expired() << "\n";           // 1
//     std::cout << "lock valid=" << (bool)wp.lock() << "\n";     // 0
// }