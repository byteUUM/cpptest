#include "BlockQueue.hpp"
#include <string>
#include <unistd.h>
using namespace my_blockqueue;
BlockQueue<int> bq;
void* Run1(void* p)
{
    string name = static_cast<const char*>(p);
    int count = 1;
    while(true)
    {
        bq.Push(count++);
        cout<<name<<" send: "<<count<<endl;
    }
}
void* Run2(void* p)
{
    string name = static_cast<const char*>(p);
    while(true)
    {
        sleep(1);
        int count = bq.Pop();
        cout<<name<<" accept: "<<count<<endl;
    }
}
int main()
{
    pthread_t tid1[5],tid2[3];
    for(int i=0;i<5;i++)
        pthread_create(&tid1[i],nullptr,Run1,(void*)"tid1");
    for(int i=0;i<3;i++)
        pthread_create(&tid2[i],nullptr,Run2,(void*)"tid2");
 
 
    for(int i=0;i<5;i++)
        pthread_join(tid1[i],nullptr);
    for(int i=0;i<3;i++)
        pthread_join(tid2[i],nullptr);
    return 0;
}
