#include <iostream>
#include <sys/types.h>
#include <socket.h>
using namespace std;
int main()
{
    int socketfd = socket(AF_INET,SOCK_DGRAM,0);
    if(socketfd<0)
    {
        exit(-1);
    }
    socket_in sn;
    sn.sin_family = AF_INET;
    sn.sin_port = htons(9201);
    sn.sin_addr.s_addr = INADDR_ANY;

    int n = bind(socketfd, (const sockaddr *)&sn, sizeof(sn));
    if (n != 0)
    {
        LOG(Level::FATAL) << "bind fial";
        exit(1);
    }
    else
    {
        LOG(Level::INFO) << "bind success";
    }
    
    return 0;
}