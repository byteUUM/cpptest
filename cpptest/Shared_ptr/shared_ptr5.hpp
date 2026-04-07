#pragma once
#include <iostream>
#include <functional>
namespace sptr
{
    using namespace std;
    template<class T>
    class my_shared_ptr
    {
        using del_t = function<void(T*)>;
    public:
        explicit my_shared_ptr(T* ptr, del_t del = [](T* ptr){
            delete ptr;
            ptr = nullptr;
        })
        :_ptr(ptr)
        ,_count(new int(1))
        ,_del(del)
        {}
        my_shared_ptr(const my_shared_ptr& other)
        {
            _count = other._count;
            *(_count)++;
            _ptr = other._ptr;
            _del = other._del;
        }
        my_shared_ptr(my_shared_ptr&& other)
        {
            _count = other._count;
            other._count = nullptr;
            _ptr = other._ptr;
            other._ptr = nullptr;
            _del = other._del;
        }
        my_shared_ptr& operator=(const my_shared_ptr& other)
        {
            if(_ptr == other._ptr) return *this;
            if(--(*_count)==0)
            {
                del(_ptr);
                delete _count;
            }
            _count = other._count;
            _ptr = other._ptr;
            _del = other._del;
            return *this;
        }
        T* get() {return _ptr;}
        T* operator->()
        {
            return _ptr;
        }
        T& operator*()
        {
            return *_ptr;
        }
        ~my_shared_ptr()
        {
            if(--(_count)==0)
            {
                _del(_ptr);
                delete _count;
            }
        }
    private:
        T* _ptr;
        int *_count;
        del_t _del;
    };
}