#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <cstring>
#include <sys/wait.h>
using namespace std;
int main()
{
    int pfd[2];
    int ret = pipe(pfd);
    if(!fork())
    {
	close(pfd[1]);
	char buffer[1024];
        ret = read(pfd[0],buffer,sizeof(buffer)-1);
        if(ret<=0)
	{
	    return 0;
	}
	buffer[ret]='\0';
	cout<<buffer<<endl;
	close(pfd[0]);
        exit(0);
    }
    close(pfd[0]);
    char s[]="hello pipe!";
    write(pfd[1],s,strlen(s));
    close(pfd[1]);
    waitpid(-1,NULL,0);
    return 0;
}
