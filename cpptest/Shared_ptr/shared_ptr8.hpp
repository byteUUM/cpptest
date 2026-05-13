#include <iostream>
#include <functional>
namespace sptr
{
    using namespace std;
    template<class T>
    class my_shared_ptr
    {
        using del_t = function<void(T*)>;
    private:
        void release()
        {
            if(!_count) return;
            if(--(*_count)==0)
            {
                _del(_ptr);
                _count = nullptr;
                _ptr = nullptr;
            }

        }
    public:
        my_shared_ptr(T* ptr, del_t del = [](T* ptr){delete ptr;})
        :_ptr(ptr)
        ,_count(ptr? new int(1):0)
        ,_del(del)
        {}
        my_shared_ptr(const my_shared_ptr& other)
        {
            _ptr = other._ptr;
            _count = other._count;
            _del = other._del;

            if(_ptr) ++(*_count);
        }
        my_shared_ptr(my_shared_ptr&& other)
        {
            _ptr = other._ptr;
            _count = other._count;
            _del = std::move(other._del);

            other._ptr = nullptr;
            other._count = nullptr;
            
        }

        my_shared_ptr<T> operator=(my_shared_ptr& other)
        {
            if(_ptr==other._ptr) return *this;
            release();
            _ptr = other._ptr;
            _count = other._count;
            _del = other._del;

            ++(*_count);
            return *this;
        }
        my_shared_ptr<T> operator=(my_shared_ptr&& other)
        {
            if(_ptr==other._ptr) return *this;
            release();
            _ptr = other._ptr;
            _count = other._count;
            _del = std::move(other._del);

            other._ptr = nullptr;
            other._count = nullptr;
            return *this;
        }

        T* get(){return _ptr;}
        T& operator*(){return *_ptr;}
        T* operator->(){return _ptr;}

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