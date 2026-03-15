#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
const int NUM = 5;
template<class T>
class BlockQueue
{
public:
    BlockQueue(int num = NUM)
    :_pnum(num)
    ,_cnum(0)
    {}
    void Push(T val)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        while(_pnum<=0)
        {
            _pcond.wait(lock);
        }
        _qu.push(val);
        _pnum--;
        _cnum++;
        if(_cnum>0) _ccond.notify_one();
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
        _pnum++;
        _cnum--;
        if(_pnum>0) _pcond.notify_one();
        return ret;
    }
private:
    std::queue<T> _qu;
    std::mutex _mutex;
    int _pnum;
    int _cnum;
    std::condition_variable _pcond;
    std::condition_variable _ccond;
};