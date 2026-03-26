#include <iostream>
#include <functional>

namespace sptr
{
    template<class T>
    class my_shared_ptr
    {
        using del_t = std::function<void(T*)>;
    public:
        my_shared_ptr(T* ptr, del_t del = [](T* p){delete p;p=nullptr;})
        :_count(new int(1))
        ,_ptr(ptr)
        ,_del(del)
        {}
        my_shared_ptr(const my_shared_ptr& x)
        {
            _count = x._count;
            (*_count)++;
            _ptr = x._ptr;
            _del = x._del;
        }
        my_shared_ptr& operator=(my_shared_ptr& x)
        {
            if(x._ptr==_ptr) return *this;
            if(*(--_count)==0)
            {
                _del(_ptr);
                delete _count;
            }
            _count = x._count;
            _ptr = x._ptr;
            _del = x._del;
        }

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
            if(--(*_count)==0)
            {
                _del(_ptr);
                delete _count;
            }
        }
    private:
        int* _count;
        T* _ptr;
        del_t _del;
    };
}