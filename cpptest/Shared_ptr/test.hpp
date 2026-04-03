#pragma once
#include <iostream>
#include <functional>

namespace swptr
{
    using namespace std;
    //控制块
    struct ControlBlock
    {
        int _strong = 0;
        int _weak = 0;
    };
    template<class T>
    class my_shared_ptr
    {
        using del_t = function<void(T*)>;
        template<class U> friend class my_weak_ptr; 
    void release()
    {
        if(!_cb) return;
        if(--_cb->_strong == 0)
        {
            _del(_ptr);
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
            ,_cb(ptr?new ControlBlock{1,0}:nullptr)
            {}
        my_shared_ptr(const my_shared_ptr& other)
            :_ptr(other._ptr)
            ,_cb(other._cb)
            {
                if(_cb) ++_cb->_strong;
            }
        my_shared_ptr(my_shared_ptr&& other)
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
        T& operator*() {return *_ptr;}
        int use_count() {return _cb?_cb->_strong:0;}
        ~my_shared_ptr()
        {
            release();
        }
    private:
        T* _ptr;
        del_t _del;
        ControlBlock* _cb;
    };
    template<class T>
    class my_weak_ptr
    {
        using del_t = function<void(T*)>;
        void release()
        {
            if(_cb&&--_cb->_weak == 0&&_cb->_strong==0)
                _del(_ptr);
        }
    public:
        my_weak_ptr()
            :_ptr(nullptr)
            ,_cb(nullptr)
            ,_del([](T* x){delete x;})
        {}
        my_weak_ptr(const my_shared_ptr<T>& sp)
            :_ptr(sp._ptr)
            ,_cb(sp._cb)
            ,_del(sp._del)
        {
            if(_cb) ++_cb->_weak;
        }
        bool expired() const {return !_cb||_cb->_strong == 0;}
        my_shared_ptr<T> lock() const
        {
            if(expired()) return my_shared_ptr<T>();
            ++_cb->_strong;
            my_shared_ptr<T> sp;
            sp._ptr = _ptr;
            sp._cb = _cb;
            return sp;
        }
        ~my_weak_ptr()
        {
            release();
        }
    private:
        T* _ptr;
        del_t _del;
        ControlBlock* _cb;
    };
}

