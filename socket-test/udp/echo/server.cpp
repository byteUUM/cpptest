#include <iostream>
#include <sys/socket.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
using namespace std;
const uint16_t port = 9001;

int main()
{
    int sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if(sockfd<0)
    {
        cout<<"socket fail!"<<endl;
        exit(1);
    }
    struct sockaddr_in sn;
    sn.sin_family = AF_INET;
    sn.sin_port = htons(port);
    sn.sin_addr.s_addr  = INADDR_ANY;
    int ret = bind(sockfd,(struct sockaddr*)&sn,sizeof(sn));
    if(ret<0)
    {
        cout<<"bind fail!"<<endl;
        exit(2);
    }
    char buffer[1024];
    socklen_t sl;
    sl = sizeof(sn);
    while(true)
    {
        //ret = recvfrom(sockfd,buffer,sizeof(buffer)-1,0,(struct sockaddr*)&sn,&sl);
        ret = recvfrom(sockfd,buffer,sizeof(buffer)-1,0,nullptr,nullptr);
        buffer[ret]=0;
        cout<<buffer<<endl;
    }
    return 0;
}