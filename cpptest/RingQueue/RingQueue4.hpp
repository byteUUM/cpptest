#include <iostream>
#include <semaphore.h>
#include <mutex>
#include <vector>
#include <thread>
namespace RingQueue_Model
{
    template<class T>
    class RingQueue
    {
    public:
        RingQueue(int sz=10)
        :_p_index(0)
        ,_c_index(0)
        ,_ringqueue(sz)
        {
            sem_init(&_p_sem,0,sz);
            sem_init(&_c_sem,0,0);
        }
        void Push(T data)
        {
            std::unique_lock<std::mutex> lock(_p_mutex);
            sem_wait(&_p_sem);
            _ringqueue[_p_index] = data;
            _p_index = (_p_index+1)%_ringqueue.size();
            sem_post(&_c_sem);
        }
        T Pop()
        {
            std::unique_lock<std::mutex> lock(_c_mutex);
            sem_wait(&_c_sem);
            T ret = _ringqueue[_c_index];
            _c_index = (_c_index+1)%_ringqueue.size();
            sem_post(&_p_sem);
            return ret;
        }
    private:
        int _p_index;
        int _c_index;
        std::mutex _p_mutex;
        std::mutex _c_mutex;
        sem_t _p_sem;
        sem_t _c_sem;
        std::vector<T> _ringqueue; 
    };
}