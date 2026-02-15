#include <iostream>
#include <signal.h>
#include <unistd.h>
using namespace std;
#if 0
void handler(int sig)
{
    cout<<sig<<"信号捕捉处理！\n";
}
int main()
{
    signal(3,handler);
    signal(2,handler);
    sigset_t mask,omask;
    sigemptyset(&mask);
    sigaddset(&mask,20);
    sigprocmask(SIG_SETMASK,&mask,&omask);
    while(true)
    {
        int ret = sleep(100);
        if(errno==EINTR)
        {
            cout<<ret<<endl;
            kill(getpid(),2);
        }
    }
    return 0;
}
#endif
#include <iostream>
#include <signal.h>
#include <unistd.h>
using namespace std;

void handler(int sig)
{
    while(true)
    {
        cout<<sig<<"信号捕捉处理！\n";
        sigset_t sg;
        sigpending(&sg);
        for(int i=31;i>=1;i--)
        {
            if(sigismember(&sg,i))
                cout<<1;
            else
                cout<<0;
        }
        cout<<endl;
        sleep(1);
    }
}
int main()
{
    signal(3,handler);
    signal(2,handler);
    sigset_t mask,omask;
    sigemptyset(&mask);
    sigaddset(&mask,20);
    sigprocmask(SIG_SETMASK,&mask,&omask);
    while(true)
    {
        int ret = sleep(100);
        sigset_t sg;
        sigpending(&sg);
        for(int i=31;i>=1;i--)
        {
            if(sigismember(&sg,i))
                cout<<1;
            else
                cout<<0;
        }
        cout<<endl;
    }
    return 0;
}
