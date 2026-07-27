#include <iostream>
#include <unistd.h>
#include <vector>
#include <mutex>
#include <thread>
using namespace std;

int count = 0;
std::mutex m1,m2;
void fun1(int id)
{
    sleep(2);
    std::lock(m1, m2);
    std::unique_lock<std::mutex> v1(m1, adopt_lock);
    std::unique_lock<std::mutex> v2(m2, adopt_lock);
    // 死锁模拟
    // std::unique_lock<std::mutex> v1(m1);
    // std::unique_lock<std::mutex> v2(m2);
    count++;
    std::cout<<id<<":"<<count<<std::endl;
}
void fun2(int id)
{
    sleep(2);
    std::lock(m1, m2);
    std::unique_lock<std::mutex> v1(m1, adopt_lock);
    std::unique_lock<std::mutex> v2(m2, adopt_lock);
    // 死锁模拟
    // std::unique_lock<std::mutex> v1(m2);
    // std::unique_lock<std::mutex> v2(m1);
    count++;
    std::cout<<id<<":"<<count<<std::endl;
}
int main()
{
    std::vector<std::thread> td(10);
    for(int i=0;i<10;i++)
    {
        td[i] = std::thread(i%2?fun1:fun2,i);
    }
    for(int i=0;i<10;i++)
    {
        td[i].join();
    }
    return 0;
}