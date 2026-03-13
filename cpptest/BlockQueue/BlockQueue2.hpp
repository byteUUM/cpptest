#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
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
        qu.push(val);
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
        T ret = qu.front();
        qu.pop();
        _pnum++;
        _cnum--;
        if(_pnum>0) _pcond.notify_one();
        return ret;
    }
private:
    std::queue<T> qu;
    int _pnum;
    int _cnum;
    std::mutex _mutex;
    std::condition_variable _pcond;
    std::condition_variable _ccond;
};