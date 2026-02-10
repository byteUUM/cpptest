#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
using namespace std;
int main()
{
    if(!fork())
    {
        execl("/usr/bin/ls","ls",NULL);
        exit(0);
    }
    if(!fork())
    {
        execlp("/home/qsy/git-test/linux-test/addr","./addr",NULL);
        exit(0);
    }
    if(!fork())
    {
        char* argv[]={"ls","-al",NULL};
        execv("/usr/bin/ls",argv);
        exit(0);
    }
    waitpid(-1,NULL,0);
    sleep(1);
    return 0;
}