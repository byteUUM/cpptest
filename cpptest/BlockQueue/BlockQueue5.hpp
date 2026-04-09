#include <iostream>
#include <queue>
#include <mutex>
#include <condition_variable>

namespace my_black_queue_model
{
    using namespace std;
    template<class T>
    class BlockQueue
    {
    public:
        BlockQueue(int num = 10)
        :_pnum(num)
        ,_cnum(0)
        {}
        void Push(const T data)
        {
            std::unique_lock<std::mutex> lock(_mutex);
            while(_pnum<=0)
            {
                _pcond.wait(lock);
            }
            _qu.push(data);
            --_pnum;
            ++_cnum;
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
            --_cnum;
            ++_pnum;
            _pcond.notify_one();
            return ret;
        }
    private:
        mutex _mutex;
        queue<T> _qu;
        int _pnum;
        int _cnum;
        condition_variable _ccond;
        condition_variable _pcond;
    };
}