#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
namespace my_black_queue_model
{
    using namespace std;
    template<class T>
    class BlockQueue
    {
    public:
        BlockQueue(size_t sz=5)
        :_pnum(sz)
        ,_cnum(0)
        {}
        void Push(const T& val)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            while(_pnum<=0)
            {
                //解锁并等待被唤醒
                _pcond.wait(lock);
            }
            _qu.push(val);
            _pnum--;
            _cnum++;
            _ccond.notify_one();
        }
        T Pop()
        {
            std::unique_lock<std::mutex> lock(_mutex);
            while(_cnum<=0)
            {
                _ccond.wait(lock);
            }
            T ret = _qu.front();
            _qu.pop();
            _cnum--;
            _pnum++;
            _pcond.notify_one();
            return ret;
        } 
    private:
        queue<T> _qu;
        mutex _mutex;
        int _pnum;
        int _cnum;
        condition_variable _pcond;
        condition_variable _ccond;
    };
}