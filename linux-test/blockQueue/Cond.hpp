#pragma once
#include <iostream>
#include <pthread.h>
#include "Mutex.hpp"
using namespace my_mutex;
namespace my_cond
{
    class Cond
    {
    public:
        Cond()
        {
            pthread_cond_init(&_cond,nullptr);
        }
        void Wait(Mutex& _mutex)
        {
            pthread_cond_wait(&_cond,_mutex.GetLock());
        }
        void Signal()
        {
            pthread_cond_signal(&_cond);
        }
        void Broadcast()
        {
            pthread_cond_broadcast(&_cond);
        }
        ~Cond()
        {
            pthread_cond_signal(&_cond);
        }
    private:
        pthread_cond_t _cond;
    };
}
