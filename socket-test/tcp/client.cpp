#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;

int main()
{
    int sockfd = socket(AF_INET,SOCK_STREAM,0);
    if(sockfd<0)
    {
        cout<<"socket fail!"<<endl;
        exit(1);
    }
    struct sockaddr_in sn = {AF_INET,htons(9002),inet_addr("115.190.60.239")};
    connect(sockfd,(struct sockaddr*)&sn,sizeof(sn));
    while(true)
    {
        string message;
        getline(std::cin,message);
        int ret = write(sockfd,message.c_str(),sizeof(message));
        if(ret<=0)
        {
            cout<<"write fail"<<endl;
            exit(2);
        }
    }
    return 0;
}