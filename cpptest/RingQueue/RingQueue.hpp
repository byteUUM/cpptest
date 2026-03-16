#pragma once
#include <iostream>
#include <pthread.h>
#include <vector>
#include <mutex>
#include <semaphore.h>

using namespace std;
namespace RingQueue_Model
{
    template <typename T>
    class RingQueue
    {
    public:
        RingQueue(size_t sz)
            : _ringQueue(sz),
              _p_index(0),
              _c_index(0)
        {
            sem_init(&_p_sem,0,sz);
            sem_init(&_c_sem,0,0);
        }
        void Push(const T &data)
        {
            sem_wait(&_p_sem);
            std::unique_lock<std::mutex> lock(_p_lock);
            _ringQueue[_p_index] = data;
            _p_index = (_p_index + 1) % _ringQueue.size();
            sem_post(&_c_sem);
        }
        T Pop()
        {
            sem_wait(&_c_sem);
            std::unique_lock<std::mutex> lock(_c_lock);
            T ret = _ringQueue[_c_index];
            _c_index = (_c_index + 1) % _ringQueue.size();
            sem_post(&_p_sem);
            return ret;
        }
 
    private:
        vector<T> _ringQueue;
        sem_t _p_sem;
        int _p_index;
        sem_t _c_sem;
        int _c_index;
        std::mutex _p_lock;
        std::mutex _c_lock;
    };
}