#include <iostream>
// #include <pthread.h>
#include <thread>
#include <unistd.h>
#include "RingQueue4.hpp"
using namespace std;
using namespace RingQueue_Model;
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
 
    //pthread_t tid1[5],tid2[3];
    thread tid1[5],tid2[3];
    for(int i=0;i<5;i++)
    {
        char buffer[128];
        snprintf(buffer,sizeof(buffer),"p_thread-%d",i+1);
        //pthread_create(&tid1[i],nullptr,fun1,(void*)buffer);
        tid1[i] = thread(fun1,(void*)buffer);
    }
    for(int i=0;i<3;i++)
    {
        char buffer[128];
        snprintf(buffer,sizeof(buffer),"p_thread-%d",i+1);
        //pthread_create(&tid2[i],nullptr,fun2,(void*)buffer);
        tid2[i] = thread(fun2,(void*)buffer);
    }
 
    for(int i=0;i<5;i++) tid1[i].join();//(tid1[i],nullptr);
    for(int i=0;i<3;i++) tid2[i].join();//pthread_join(tid2[i],nullptr);
    return 0;
}