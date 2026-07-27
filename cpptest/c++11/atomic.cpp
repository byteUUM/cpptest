#include <thread>
#include <iostream>
#include <unistd.h>
#include <atomic>
using namespace std;

std::atomic<int> cas = 0;
void fun(int id)
{
    std::cout<<this_thread::get_id()<<" start"<<endl;
    int n = 5;
    while(n--)
    {
        cas++;
        std::cout<<id<<": "<<cas<<endl;
        sleep(1);
    }
}

int main()
{
    std::thread t1(fun,1);
    std::thread t2(fun,2);
    t1.join();
    t2.join();
    return 0;
}