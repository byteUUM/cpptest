#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <unistd.h>
template<class T>
class BlockQueue
{
public:
    BlockQueue(int num=5)
    :_pnum(num)
    ,_cnum(0){}
    void Push(T data)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        while(_pnum<=0)
        {
            _pcond.wait(lock);
        }
        qu.push(data);
        _cnum++;
        _pnum--;
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
        _cnum--;
        _pnum++;
        if(_pnum>0) _pcond.notify_one();
        return ret;
    }
private:
    std::queue<T> qu;
    std::mutex _mutex;
    int _pnum;
    int _cnum;
    std::condition_variable _ccond;
    std::condition_variable _pcond;
};