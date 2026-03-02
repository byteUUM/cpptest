#include <iostream>
#include <pthread.h>
#include <queue>
#include "Cond.hpp"
using namespace std;
using namespace my_cond;
using namespace my_mutex;
const size_t NUM = 5;
namespace my_blockqueue
{
    template <typename T>
    class BlockQueue
    {
    public:
        BlockQueue(int sz = NUM)
            : _pnum(sz),
              _cnum(0)
        {}
        void Push(T data)
        {
            _mutex.Lock();
            while (_pnum <= 0)
            {
                _pcond.Wait(_mutex);
            }
            qu.push(data);
            if (_cnum <= 0)
                _ccond.Signal();
            _pnum--, _cnum++;
            _mutex.Unlock();
        }
        T Pop()
        {
            _mutex.Lock();
            while (_cnum <= 0)
            {
                _ccond.Wait(_mutex);
            }
            T ret = qu.front();
            qu.pop();
            if (_pnum <= 0)
                _pcond.Signal();
            _cnum--, _pnum++;
            _mutex.Unlock();
            return ret;
        }
 
    private:
        queue<T> qu;
        int _pnum; // 生产者可以生产的数量
        int _cnum;
        Mutex _mutex;
        Cond _pcond;
        Cond _ccond;
    };
}
 
