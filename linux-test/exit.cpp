#include <iostream>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;
void test_wait()
{
    int pid = fork();
    if(pid==-1)
        cout<<"progress create fail"<<endl;
    if(pid==0)
    {
        cout<<getpid()<<": child runing"<<endl;
        //kill(getpid(),9);
    }
    else
    {
        int state;
        int id = wait(&state);
        if((state&0x7F)==0)
        {
            cout<<"success pid: "<<id<<endl;
            cout<<"exit code: "<<(state&(0xFF<<8))<<endl;
        }
        else
        {
            cout<<"exit sig: "<<(state&(0x7F))<<endl;
        }
    }
}
void test_waitpid()
{
    for(int i=0;i<10;i++)
    {
        if(!fork())
        {
            sleep(3);
            exit(0);
        }
    }
    int n=10;
    while(n)
    {
        int ret = waitpid(-1,NULL,WNOHANG);
        if(ret == -1)
        {
            cout<<"waitpid fail"<<endl;
            exit(-1);
        }
        if(!ret) n--;
        sleep(3);
    }
}
int main()
{
    test_waitpid();
    return 0;
}