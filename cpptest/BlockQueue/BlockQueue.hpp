#include <thread>
#include <iostream>
#include <pthread.h>
#include <queue>
#include <mutex>
#include <condition_variable>
using namespace std;
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
            std::unique_lock<std::mutex> lock(_mutex);
            while (_pnum <= 0)
            {
                _pcond.wait(lock);
            }
            qu.push(data);
            if (_cnum <= 0)
                _ccond.notify_one();
            _pnum--, _cnum++;
        }
        T Pop()
        {
            std::unique_lock<std::mutex> lock(_mutex);
            while (_cnum <= 0)
            {
                _ccond.wait(lock);
            }
            T ret = qu.front();
            qu.pop();
            if (_pnum <= 0)
                _pcond.notify_one();
            _cnum--, _pnum++;

            return ret;
        }
 
    private:
        queue<T> qu;
        int _pnum; // 生产者可以生产的数量
        int _cnum;
        std::mutex _mutex;
        std::condition_variable _pcond;
        std::condition_variable _ccond;
    };
}
 