#include <iostream>
#include <functional>

namespace sptr
{
    template<class T>
    class my_shared_ptr
    {
        using del_t = std::function<void(T*)>;
    public:
        my_shared_ptr(T* ptr,del_t del = [](T* ptr){delete ptr;ptr=nullptr;})
        :_count(new int(1))
        ,_ptr(ptr)
        ,_del(del)
        {}
        my_shared_ptr(const my_shared_ptr& msp)
        {
            _count = msp._count;
            (*_count)++;
            _ptr = msp._ptr;
            _del = msp._del;
        }
        my_shared_ptr& operator=(const my_shared_ptr& msp)
        {
            if(msp._ptr==_ptr) return  *this;
            if(--(*_count)==0)
            {
                _del(_ptr);
                delete _count;
                _count=nullptr;
            }
            _count = msp._count;
            (*_count)++;
            _ptr = msp._ptr;
            _del = msp._ptr;
            return *this;
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
                _count = nullptr;
            }
        }
    private:
        int* _count;
        T* _ptr;
        del_t _del;
    };
}