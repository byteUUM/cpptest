#pragma once
#include <iostream>
#include <pthread.h>
#include <vector>
#include "Sem.hpp"
#include "Mutex.hpp"
using namespace std;
using namespace my_mutex;
namespace my_RingQueue
{
    template <typename T>
    class RingQueue
    {
    public:
        RingQueue(size_t sz)
            : _ringQueue(sz),
              _p_sem(sz),
              _p_index(0),
              _c_sem(0),
              _c_index(0)
        {
        }
        void Push(const T &data)
        {
            _p_sem.P();
            LockGuard lock(_p_lock);
            _ringQueue[_p_index] = data;
            _p_index = (_p_index + 1) % _ringQueue.size();
            _c_sem.V();
        }
        T Pop()
        {
            _c_sem.P();
            LockGuard lock(_c_lock);
            T ret = _ringQueue[_c_index];
            _c_index = (_c_index + 1) % _ringQueue.size();
            _p_sem.V();
            return ret;
        }
 
    private:
        vector<T> _ringQueue;
        Sem _p_sem;
        int _p_index;
        Sem _c_sem;
        int _c_index;
        Mutex _p_lock;
        Mutex _c_lock;
    };
}

#include <iostream>
#include "RingQueue.h"
#include <pthread.h>
#include <unistd.h>
using namespace std;
using namespace my_RingQueue;
RingQueue<int> rq(5);
void* fun1(void* p)
{
    string name = static_cast<char*>(p);
    int data = 1;
    while(true)
    {
        sleep(1);
        rq.Push(data++);
        cout<<name<<':'<<data-1<<endl;
    }
    return nullptr;
}
void* fun2(void* p)
{
    string name = static_cast<char*>(p);
    int val;
    while(true)
    {
        val = rq.Pop();
        cout<<name<<':'<<val<<endl;
    }
    return nullptr;
}
int main()
{
 
    pthread_t tid1[5],tid2[3];
    for(int i=0;i<5;i++)
    {
        char buffer[128];
        snprintf(buffer,sizeof(buffer),"p_thread-%d",i+1);
        pthread_create(&tid1[i],nullptr,fun1,(void*)buffer);
    }
    for(int i=0;i<3;i++)
    {
        char buffer[128];
        snprintf(buffer,sizeof(buffer),"p_thread-%d",i+1);
        pthread_create(&tid2[i],nullptr,fun2,(void*)buffer);
    }
 
    for(int i=0;i<5;i++)
        pthread_join(tid1[i],nullptr);
    for(int i=0;i<3;i++)
        pthread_join(tid2[i],nullptr);
    return 0;
}