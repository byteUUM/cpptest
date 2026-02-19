#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <thread>
#include <string>
using namespace std;
int main()
{
    int sock_fd = socket(AF_INET,SOCK_STREAM,0);
    //返回值检验
    struct sockaddr_in sn={AF_INET,htons(9003),inet_addr("115.190.60.239")};
    int ret = connect(sock_fd,(struct sockaddr*)&sn,sizeof(sn));
    while(true)
    {
        string message;
        getline(std::cin,message);
        int ret = write(sock_fd,message.c_str(),message.size());
        if(ret<=0) exit(1);
    }
    return 0;
}