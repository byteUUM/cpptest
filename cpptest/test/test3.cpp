#include <iostream>
#include <unistd.h>
#include <sys/types.h>
using namespace std;
// int main()
// {
//     int a=0;
//     int& p1=a;
//     int& p2=a;
//     int& p3=a;
//     cout<<&p1<<" "<<&p2<<" "<<&p3<<endl;
//     return 0;
// }

//僵尸进程
// int main()
// {
//     int pid = fork();
//     if(pid==0) cout<<"子进程"<<endl;
//     else
//     {
//         cout<<"父进程"<<endl;
//         sleep(30);
//     }
//     return 0;
// }

//孤儿进程
int main()
{
    int pid = fork();
    if(pid==0)
    {
        cout<<"子进程"<<endl;
        sleep(30);
    }
    else
    {
        cout<<"父进程"<<endl;
    }
    return 0;
}