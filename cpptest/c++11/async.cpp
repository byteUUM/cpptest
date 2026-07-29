#include <iostream>
#include <future>
#include <thread>
using namespace std;

int fun(int id)
{
    std::cout<<id<<std::endl;
    return id+10;
}
void task(std::promise<int> prms)
{
    prms.set_value(666);
}
void test1()
{
    //使用缺省参数，默认为std::launch::async
    //std::future ret = std::async(std::launch::deferred,fun,1);
    std::future ret = std::async(std::launch::deferred,fun,1);
    std::cout<<ret.get()<<std::endl;
}
void test2()
{
    std::promise<int> prms;
    std::future<int> ret = prms.get_future();
    std::thread th(task, std::move(prms));
    std::cout<<ret.get()<<std::endl;
    th.join();
}
int main()
{
    test2();
    return 0;
}