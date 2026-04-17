#pragma once
#include <iostream>
#include <functional>

namespace sptr
{
    template<class T>
    class my_shared_ptr
    {
        using del_t = std::function<void(T*)>;
    private:
        //分别统一管理，操作一致
        void release()
        {
            if(_count&&--(*_count)==0)
            {
                _del(_ptr);
                delete _count;
            }
            _ptr = nullptr;
            _count = nullptr;
        }
    public:
        //防止隐式构造，以至于误用
        explicit my_shared_ptr(T* ptr, del_t del = [](T* x){delete x;})
        :_ptr(ptr),
        _count(ptr?new int(1):nullptr),
        _del(del)
        {}
        my_shared_ptr(const my_shared_ptr<T>& other)
        {
            _count = other._count;
            _ptr = other._ptr;
            _del = other._del;
            //other可能被move或本身为nullptr
            if(_count) ++(*_count);
        }
        my_shared_ptr(my_shared_ptr<T>&& other)
        {
            _ptr = other._ptr;
            _count = other._count;
            _del = std::move(other._del);

            other._ptr = nullptr;
            other._count = nullptr;
        }
        my_shared_ptr<T>& operator=(const my_shared_ptr<T>& other)
        {
            if(other._ptr==_ptr) return *this;
            
            release();
            
            _count = other._count;
            _ptr = other._ptr;
            _del = other._del;
            if(_count) ++(*_count);
            return *this;
        }
        my_shared_ptr<T>& operator=(my_shared_ptr<T>&& other)
        {
            if(other._ptr==_ptr) return *this;

            release();

            _count = other._count;
            _ptr = other._ptr;
            _del = other._del;

            other._ptr = nullptr;
            other._count = nullptr;
            _del = std::move(other._del);
            return *this;
        }
        T* get() {return _ptr;}
        T& operator*() {return *_ptr;}
        T* operator->() {return _ptr;}

        //让const my_shared_ptr<T>类型也能调用，如果没有就是在放大权限不和理也不合法。
        T* get() const {return _ptr;}
        T& operator*() const {return *_ptr;}
        T* operator->() const {return _ptr;}
        ~my_shared_ptr()
        {
            release();
        }
    private:
        T* _ptr;
        int* _count;
        del_t _del;
    };
}