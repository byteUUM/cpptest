#pragma once
#include <iostream>
#include <functional>
#include <vector>
using namespace std;

void PrintLog()
{
    std::cout << "我是一个打印日志的任务" << std::endl;
}
 
void Download()
{
    std::cout << "我是一个下载的任务" << std::endl;
}
 
void Upload()
{
    std::cout << "我是一个上传的任务" << std::endl;
}
class Task
{
public:
    Task()
    {
        srand((unsigned int)time(NULL));
    }
    void push(void(*fun)())
    {
        _tasks.push_back(fun);
    }
    int get_code()
    {
        return rand()%_tasks.size();
    }
    void running(int code)
    {
        _tasks[code]();
    }
private:
    vector<function<void()>> _tasks;
};