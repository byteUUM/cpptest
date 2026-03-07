#pragma once
#include <iostream>
#include <pthread.h>
#include <vector>
#include "Sem.hpp"
#include "Mutex.hpp"
using namespace std;
using namespace my_mutex;
namespace my_RingQueue
{
    template <typename T>
    class RingQueue
    {
    public:
        RingQueue(size_t sz)
            : _ringQueue(sz),
              _p_sem(sz),
              _p_index(0),
              _c_sem(0),
              _c_index(0)
        {
        }
        void Push(const T &data)
        {
            _p_sem.P();
            LockGuard lock(_p_lock);
            _ringQueue[_p_index] = data;
            _p_index = (_p_index + 1) % _ringQueue.size();
            _c_sem.V();
        }
        T Pop()
        {
            _c_sem.P();
            LockGuard lock(_c_lock);
            T ret = _ringQueue[_c_index];
            _c_index = (_c_index + 1) % _ringQueue.size();
            _p_sem.V();
            return ret;
        }
 
    private:
        vector<T> _ringQueue;
        Sem _p_sem;
        int _p_index;
        Sem _c_sem;
        int _c_index;
        Mutex _p_lock;
        Mutex _c_lock;
    };
}
