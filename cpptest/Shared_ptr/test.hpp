#pragma once
#include <iostream>
#include <function>

namespace swptr
{
    //控制块
    struct ControlBlock
    {
        int _string = 0;
        int _weak = 0;
    };
    template<class T>
    class my_shared_ptr
    {
        using del_t = function<void(T*)>;
        template<clss U> friend class my_weak_ptr; 
    void release()
    {
        if(!_cb) return;
        if(--_cb->_strong == 0)
        {
            del(_ptr);
            _ptr = nullptr;
            if(_cb->_weak == 0)
            {
                delete _cb;
                _cb = nullptr;
            }
        }
    }
    public:
        //放在delete重复释放
        explicit my_shared_ptr(T* ptr,del_t del = [](T* x){delete x;})
            :_ptr(ptr)
            ,_cb(ptr?new ControllBlock(1,0):nullptr)
            {}
        my_shared_ptr(const shared_ptr& other)
            :_ptr(other._ptr)
            ,_cb(other._cb)
            {
                if(_cb) ++_cb->_string;
            }
        my_shared_ptr(shared_ptr&& other)
            :_ptr(other._ptr)
            ,_cb(other._cb)
            {
                other._ptr = nullptr;other._cb = nullptr;
            }
        my_shared_ptr& operator=(const my_shared_ptr& other)
        {
            if(_ptr==other._ptr) return *this;
            if(_cb&&--_cb->_strong==0)
            {
                _del(_ptr);
                delete _cb;
                _cb = nullptr;
            }
            _ptr=other._ptr;
            _cb=other._cb;
            if(_cb) ++_cb->_strong;
            return *this;
        }
        T* get() {return _ptr;}
        T* operator->() {return _ptr;}
        T& operator*() {return _ptr;}
        int use_count() {return _cb?_cb->_strong:0;}
        ~shared_ptr()
        {
            release();
        }
    private:
        T* _ptr;
        del_t _del;
        ControllBlock* _cb;
    }
    template<class U>
    class my_weak_ptr
    {
    public:
    private:
        T* _ptr;
        Con
    };
}

