#pragma once
#include <iostream>
#include <functional>
using namespace std;

namespace sptr
{
    template <class T>
    class my_shared_ptr
    {
    using del_t = function<void(T*)>;
    private:
        void release()
        {
            if(!_ptr) return;
            if(--_count==0)
            {
                _del(_ptr);
                _del = nullptr;
                _count = nullptr;
            }
        }

    public:
        my_shared_ptr(T* ptr, del_t del=[](T* ptr){delete ptr;})
        :_ptr(ptr)
        ,_count(ptr?new int(1):nullptr)
        ,_del(del)
        {}
        my_shared_ptr(my_shared_ptr<T>& other)
        {
            _count = other._count;
            _ptr = other._ptr;
            _del = other._del;
            if(_count) ++_count;
        }
        my_shared_ptr(my_shared_ptr<T>&& other)
        {
            _count = other._count;
            _ptr = other._ptr;
            _del = move(other._del);

            other._count = nullptr;
            other._ptr = nullptr;
        }
        my_shared_ptr<T>& operator=(my_shared_ptr<T>& other)
        {
            if(_ptr == other._ptr) return *this;
            release();
            _ptr = other._ptr;
            _count = other._count;
            _del = other._del;
            ++_count;
            return *this;
        }
        my_shared_ptr<T>& operator=(my_shared_ptr<T>&& other)
        {
           if(_ptr == other._ptr) return *this;
           release();
           _ptr = other._ptr;
           _count = other._count;
           _del = move(other._del);
           
           _ptr = nullptr;
           _count = nullptr;
           return *this;
        }
        T* get(){return _ptr;}
        T* get() const {return _ptr;}
        T* operator->(){return _ptr;}
        T* operator->() const {return _ptr;}
        T& operator*() {return *_ptr;}
        T& operator*() const {return *_ptr;}
    private:
        T* _ptr;
        int* _count;
        del_t _del;
    };

}
