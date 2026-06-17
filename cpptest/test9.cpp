#pragma once
#include <utility>   // std::move, std::swap
#include <cstddef>   // nullptr_t

// ============================================================
// UniquePtr<T>：独占式智能指针（模拟 std::unique_ptr）
//
// 核心语义：
//   1. 独占资源所有权 —— 不允许拷贝，只允许移动（move-only）
//   2. 析构时自动释放资源（RAII）
//   3. 支持自定义删除器（Deleter），默认是 delete
//   4. 零开销抽象：不维护引用计数，只是一个裸指针的包装
// ============================================================

// 默认删除器：调用 delete
template <typename T>
struct DefaultDeleter {
    void operator()(T* ptr) const {
        delete ptr;
    }
};

// 数组版本的默认删除器（特化）：调用 delete[]
template <typename T>
struct DefaultDeleter<T[]> {
    void operator()(T* ptr) const {
        delete[] ptr;
    }
};

template <typename T, typename Deleter = DefaultDeleter<T>>
class UniquePtr {
public:
    // ---------- 构造函数 ----------
    explicit UniquePtr(T* ptr = nullptr) noexcept : ptr_(ptr) {}

    UniquePtr(T* ptr, Deleter deleter) noexcept
        : ptr_(ptr), deleter_(std::move(deleter)) {}

    UniquePtr(std::nullptr_t) noexcept : ptr_(nullptr) {}

    // ---------- 禁止拷贝（独占语义的关键） ----------
    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    // ---------- 移动构造 / 移动赋值 ----------
    UniquePtr(UniquePtr&& other) noexcept
        : ptr_(other.ptr_), deleter_(std::move(other.deleter_)) {
        other.ptr_ = nullptr; // 转移所有权后，原对象必须置空
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            reset(other.ptr_);          // 先释放自己持有的资源
            deleter_ = std::move(other.deleter_);
            other.ptr_ = nullptr;       // 原对象放弃所有权
        }
        return *this;
    }

    // 支持从派生类的 UniquePtr 移动构造（协变转换），例如：
    // UniquePtr<Base> base = UniquePtr<Derived>(new Derived());
    template <typename U, typename UDeleter>
    UniquePtr(UniquePtr<U, UDeleter>&& other) noexcept
        : ptr_(other.release()), deleter_(std::move(other.get_deleter())) {}

    // ---------- 析构函数 ----------
    ~UniquePtr() {
        if (ptr_) {
            deleter_(ptr_);
        }
    }

    // ---------- 核心接口 ----------

    // 释放所有权，返回裸指针，自己不再管理该资源
    T* release() noexcept {
        T* tmp = ptr_;
        ptr_ = nullptr;
        return tmp;
    }

    // 重新指向新资源，先释放旧资源
    void reset(T* new_ptr = nullptr) noexcept {
        if (ptr_ != new_ptr) {
            T* old = ptr_;
            ptr_ = new_ptr;
            if (old) {
                deleter_(old);
            }
        }
    }

    void swap(UniquePtr& other) noexcept {
        std::swap(ptr_, other.ptr_);
        std::swap(deleter_, other.deleter_);
    }

    T* get() const noexcept { return ptr_; }
    Deleter& get_deleter() noexcept { return deleter_; }
    const Deleter& get_deleter() const noexcept { return deleter_; }

    // ---------- 运算符重载 ----------
    T& operator*() const { return *ptr_; }
    T* operator->() const noexcept { return ptr_; }
    explicit operator bool() const noexcept { return ptr_ != nullptr; }

private:
    T* ptr_ = nullptr;
    Deleter deleter_{};
};

// ---------- 数组特化版本：UniquePtr<T[]> ----------
// 区别仅在于 operator[] 以及默认删除器使用 delete[]
template <typename T, typename Deleter>
class UniquePtr<T[], Deleter> {
public:
    explicit UniquePtr(T* ptr = nullptr) noexcept : ptr_(ptr) {}

    UniquePtr(const UniquePtr&) = delete;
    UniquePtr& operator=(const UniquePtr&) = delete;

    UniquePtr(UniquePtr&& other) noexcept : ptr_(other.ptr_) {
        other.ptr_ = nullptr;
    }

    UniquePtr& operator=(UniquePtr&& other) noexcept {
        if (this != &other) {
            reset(other.ptr_);
            other.ptr_ = nullptr;
        }
        return *this;
    }

    ~UniquePtr() {
        if (ptr_) deleter_(ptr_);
    }

    T& operator[](size_t index) const { return ptr_[index]; }

    T* release() noexcept {
        T* tmp = ptr_;
        ptr_ = nullptr;
        return tmp;
    }

    void reset(T* new_ptr = nullptr) noexcept {
        if (ptr_ != new_ptr) {
            T* old = ptr_;
            ptr_ = new_ptr;
            if (old) deleter_(old);
        }
    }

    T* get() const noexcept { return ptr_; }
    explicit operator bool() const noexcept { return ptr_ != nullptr; }

private:
    T* ptr_ = nullptr;
    Deleter deleter_{};
};

// ---------- 工厂函数（类似 std::make_unique） ----------
template <typename T, typename... Args>
UniquePtr<T> MakeUnique(Args&&... args) {
    return UniquePtr<T>(new T(std::forward<Args>(args)...));
}
