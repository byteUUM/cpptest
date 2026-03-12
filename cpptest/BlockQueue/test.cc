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
    std::thread th1[5],th2[3];
    for(int i=0;i<5;i++)
        th1[i] = std::thread(Run1,(void*)"tid1");
    for(int i=0;i<3;i++)
        th2[i] = std::thread(Run2,(void*)"tid2");
 

    for(int i=0;i<5;i++) th1[i].join();
    for(int i=0;i<3;i++) th2[i].join();
    return 0;
}