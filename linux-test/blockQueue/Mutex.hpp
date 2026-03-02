#pragma once
#include <iostream>
#include <pthread.h>
#include <string>
#include <unistd.h>
using namespace std;
namespace my_mutex
{
    class Mutex
    {
    public:
        Mutex()
        {
            pthread_mutex_init(&_mutex, nullptr);
        }
        void Lock()
        {
            pthread_mutex_lock(&_mutex);
        }
        void Unlock()
        {
            pthread_mutex_unlock(&_mutex);
        }
        pthread_mutex_t* GetLock()
        {
            return &_mutex;
        }
        ~Mutex()
        {
            pthread_mutex_destroy(&_mutex);
        }
 
    private:
        pthread_mutex_t _mutex;
    };
    class LockGuard
    {
    public:
        LockGuard(Mutex& lock)
            : _lock(lock)
        {
            _lock.Lock();
        }
        ~LockGuard()
        {
            _lock.Unlock();
        }
    private:
        Mutex& _lock;
    };
}

