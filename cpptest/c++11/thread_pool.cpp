#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <queue>
using namespace std;

class thread_pool
{
    using task_func_t = function<void(void*)>;
private:
    thread_pool(int size)
    :_size(size)
    ,_stop(false)
    {
        for(int i = 0; i < size; i++)
        {
            _pools.emplace_back([this](){
                while(true)
                {
                    task_func_t tk;
                    void* data = nullptr;
                    {
                        unique_lock<std::mutex> lock(_mutex);
                        _cv.wait(lock, [this](){ return _stop || !_tasks.empty(); });
                        if(_stop && _tasks.empty())
                            return;
                        tk = _tasks.front().first;
                        data = _tasks.front().second;
                        _tasks.pop();
                    }
                    tk(data);
                }
            });
        }
    }
public:
    static thread_pool* create(int size)
    {
        if(_pool_this == nullptr)
        {
            lock_guard<std::mutex> lock(_mutex_static);
            if(_pool_this == nullptr)
                _pool_this = new thread_pool(size);
        }
        return _pool_this;
    }
    void push(task_func_t func, void* data)
    {
        {
            lock_guard<std::mutex> lock(_mutex);
            _tasks.push({func, data});
        }
        _cv.notify_one();
    }
    ~thread_pool()
    {
        _stop = true;
        _cv.notify_all();
        for(int i = 0; i < _size; i++)
        {
            if(_pools[i].joinable())
                _pools[i].join();
        }
    }
private:
    vector<thread> _pools;
    int _size;
    std::mutex _mutex;
    atomic<bool> _stop;
    condition_variable _cv;
    
    // 任务队列
    queue<pair<task_func_t, void*>> _tasks;

    static thread_pool* _pool_this;
    static mutex _mutex_static;
};

thread_pool* thread_pool::_pool_this = nullptr;
mutex thread_pool::_mutex_static;

// 简单测试
int main()
{
    thread_pool* pool = thread_pool::create(4);
    
    for(int i = 0; i < 10; i++)
    {
        int* data = new int(i);
        pool->push([](void* arg){
            int* p = (int*)arg;
            cout << "task " << *p << " executed by thread " << this_thread::get_id() << endl;
            delete p;
        }, data);
    }
    
    // 等待任务执行完毕
    this_thread::sleep_for(chrono::seconds(1));
    
    delete pool; // 优雅关闭
    cout << "thread pool destroyed" << endl;
    return 0;
}

/*
class thread_pool
{
    using task_func_t = function<void(void*)>;
private:
    thread_pool(int size)
    :_size(size)
    {
        for(int i = 0; i < size; i++)
        {
            _pools.push_back(std::thread([](){
                while(true)
                {
                    {
                        lock_guard<std::mutex> _lock(_mutex);
                        while(_tasks.empty())
                        {}
                    }
                    task_func_t tk = queue.front().first;
                    data = queue.front().second;
                    queue.pop();
                    tk(data);
                }

            }))
        }
    }
public:
    thread_pool* reate(int size)
    {
        if(this_thread==nullptr)
        {
            std::lock_guard<std::mutex> lock(_mutex);
            if(this_thread==nullptr)
                _pools = new thread_pool(size);
        }
        return this_thread;
    }
    bool push(task_func_t func, void* data)
    {
        _tasks.push({func,data})
    }
    ~thread_pool()
    {
        for(int i=0; i<_size;i++)
        {
            _pools[i].join();
        }
    }
private:
    std::vector<thread> _pools;
    int _size;
    std::mutex _mutex;
    int _count;
    
    // 任务队列
    std::queue _tasks;

    static thread_pool* _pool_this;
};
*/
