#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
using namespace std;

class thread_pool
{
private:
    thread_pool(int size)
    :_size(size)
    {}
public:
    // create(int size)
    // {
    //     std::lock_guard<std::mutex> lock(_mutex);
    //     if(this_thread==nullptr)
    //     {
    //         _pools = new thread_pool(size);
    //     }
    //     return this_thread;
    // }
    create(int size)
    {
        if(this_thread==nullptr)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if(this_thread==nullptr)
                _pools = new thread_pool(size);
        }
        return this_thread;
    }
private:
    std::vector<thread> _pools;
    int _size;
    std::mutex _mutex;
    static thread_pool* _pools;
};