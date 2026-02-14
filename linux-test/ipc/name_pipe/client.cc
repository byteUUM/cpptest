#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <cstring>
using namespace std;
int main(int argc ,char* argv[])
{
    if(argc!=2)
    {
        cout<<"input error"<<endl;
        return 0;
    }
    char* filename = argv[1];
    mkfifo(filename,0660);
    int fd = open(filename,O_RDONLY);
    char buffer[1024];
    memset(buffer,0,sizeof(buffer));
    while(true)
    {
        read(fd,buffer,sizeof(buffer)-1);
        cout<<buffer<<endl;
        sleep(2);
    }
    return 0;
}