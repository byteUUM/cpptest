#include <iostream>
#include <functional>

namespace sptr
{
    template<class T>
    class my_shared_ptr
    {
        using del_t = std::function<void(T* ptr)>;
    private:
        void release()
        {
            if(!_count) return;
            if(--(*_count)==0)
            {
                _del(_ptr);
                delete _count;
                _count = nullptr;
                _ptr = nullptr;
            }
        }
    public:
        my_shared_ptr(T* ptr, del_t del=[](T* ptr){delete ptr;})
        :_ptr(ptr)
        ,_del(del)
        ,_count(ptr?new int(1):nullptr)
        {}
        my_shared_ptr(const my_shared_ptr<T>& other)
        {
            _ptr = other._ptr;
            _count = other._count;
            if(_count) ++(*_count);
            _del = other._del;
        }
        my_shared_ptr(my_shared_ptr<T>&& other)
        {
            _ptr = other._ptr;
            _count = other._count;
            _del = std::move(other._del);

            other._ptr = nullptr;
            other._count = nullptr;
        }
        my_shared_ptr& operator=(const my_shared_ptr<T>& other)
        {
            if(_ptr == other._ptr) return *this;
            release();
            _ptr = other._ptr;
            _count = other._count;
            _del = other._del;
            if(_count) ++(*_count);

            return *this;
        }
        my_shared_ptr& operator=(my_shared_ptr<T>&& other)
        {
            if(_ptr == other._ptr) return *this;
            release();
            _ptr = other._ptr;
            _count = other._count;
            _del = std::move(other._del);

            return *this;
        }

        T* get() {return _ptr;}
        T* operator->() {return _ptr;}
        T& operator*() {return *_ptr;}

        T* get() const {return _ptr;}
        T* operator->() const {return _ptr;}
        T& operator*() const {return *_ptr;}

    private:
        int* _count;
        T* _ptr;
        del_t _del;
    };
}