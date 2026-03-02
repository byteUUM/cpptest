#pragma once
#include <semaphore.h>
#define DEF 1
class Sem
{
public:
    Sem(int val = DEF)
    {
        sem_init(&_sem,0,val);//？？
    }
    void P()
    {
        sem_wait(&_sem);
    }
    void V()
    {
        sem_post(&_sem);
    }
    ~Sem()
    {
        sem_destroy(&_sem);
    }
private:
    sem_t _sem;
};
