#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
using namespace std;

//SIGCHID信号测试
#if 0
int main()
{
    if(!fork())
    {
        exit(0);
    }   
    while(true)
    {
        sleep(1);
    }
    return 0;
}
int main()
{
    signal(SIGCHLD,SIG_IGN);
    if(!fork())
    {
        exit(0);
    }   
    while(true)
    {
        sleep(1);
    }
    return 0;
}

#endif
void handler(int sig)
{
    sleep(15);
    waitpid(-1,nullptr,0);
}
int main()
{
    signal(SIGCHLD,handler);
    if(!fork())
    {
        exit(0);
    }   
    while(true)
    {
        sleep(1);
    }
    return 0;
}