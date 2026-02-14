#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
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
    int fd = open(filename,O_WRONLY);
    char i='A';
    while(true)
    {
        write(fd,&i,sizeof(char));
        
        if(i=='z') i='A';
        else i++;
        sleep(1);
    }
    return 0;
}