#include <semaphore.h>
#include <thread>
#include <iostream>
#include <vector>
#include <mutex>
using namespace std;
// void test()
// {
//     std::binary_semaphore sem(0);
//     std::thread t([&](){
//         sem.acquire();
//         std::cout<<"收到通知\n";
//     });
//     std::cout<<"主线程...\n";
//     sem.release();//通知子线程
//     t.join();
// }
template<class T>
class ringqueue
{
public:
    ringqueue(int sz)
    :_ringqueue(sz)
    ,_p_index(0)
    ,_c_index(0)
    {
        sem_init(&_p_sem,0,sz);
        sem_init(&_c_sem,0,0);
    }
    void push(T val)
    {
        lock_guard<std::mutex> lock(_p_mtx);
        sem_wait(&_p_sem);
        _ringqueue[_p_index] = val;
        _p_index = (_p_index+1)%_ringqueue.size();
        sem_post(&_c_sem);
    }
    T pop()
    {
        lock_guard<std::mutex> lock(_c_mtx);
        sem_wait(&_c_sem);
        T ret = _ringqueue[_c_index];
        _c_index = (_c_index+1)%_ringqueue.size();
        sem_post(&_p_sem);
        return ret;
    }
private:
    mutex _p_mtx;
    mutex _c_mtx;
    vector<T> _ringqueue;
    int _p_index;
    int _c_index;
    sem_t _p_sem;
    sem_t _c_sem;
};

namespace RingQueue_Model
{
    template<class T>
    class RingQueue
    {
    public:
        // 留一个空位：实际容量 = sz，数组大小 = sz + 1
        RingQueue(int sz)
        :_ringqueue(sz + 1)
        ,_p_index(0)
        ,_c_index(0)
        {}
        bool Push(const T& val)
        {
            if (IsFull()) return false;
            _ringqueue[_p_index] = val;
            _p_index = (_p_index + 1) % _ringqueue.size();
            return true;
        }
        bool Pop(T& out)
        {
            if (IsEmpty()) return false;
            out = _ringqueue[_c_index];
            _c_index = (_c_index + 1) % _ringqueue.size();
            return true;
        }
        bool IsEmpty() const { return _p_index == _c_index; }
        bool IsFull() const { return (_p_index + 1) % _ringqueue.size() == _c_index; }
        int Size() const { return (_p_index - _c_index + _ringqueue.size()) % _ringqueue.size(); }
        int Space() const { return _ringqueue.size() - 1 - Size(); }
    private:
        std::vector<T> _ringqueue;
        int _p_index;
        int _c_index;
    };
}

void test2()
{
    const int P = 3, C = 3, N = 1000;   // 3 生产者、3 消费者，每个生产 1000 个
    ringqueue<int> q(5);                // 容量小，容易触发队满/队空阻塞

    mutex res_mtx;
    long long sum = 0;                  // 消费到的总和
    int count = 0;                      // 消费到的总个数

    vector<thread> ps, cs;
    for (int i = 0; i < P; i++)
        ps.emplace_back([&]{
            for (int j = 1; j <= N; j++) q.push(j);
        });
    for (int i = 0; i < C; i++)
        cs.emplace_back([&]{
            for (int j = 0; j < N; j++) {
                int v = q.pop();
                lock_guard<mutex> lk(res_mtx);
                sum += v; count++;
            }
        });

    for (auto& t : ps) t.join();
    for (auto& t : cs) t.join();

    long long expect = (long long)P * N * (N + 1) / 2;
    cout << "消费个数: " << count << " (期望 " << P * N << ")\n";
    cout << "总和: " << sum << " (期望 " << expect << ")\n";
    cout << (count == P * N && sum == expect ? "test2 通过" : "test2 失败") << endl;
}

int main()
{
    test2();
    return 0;
}


