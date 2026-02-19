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
    while(true)
    {
        string message;
        getline(std::cin,message);
        sendto(sockfd,message.c_str(),message.size(),0,(struct sockaddr*)&sn,sizeof(sn));
    }
    close(sockfd);
    return 0;
}


// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <unistd.h>
// #include <arpa/inet.h>

// int main() {
//     int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    
//     struct sockaddr_in addr = {AF_INET, htons(8888), inet_addr("127.0.0.1")};
//     connect(sock_fd, (struct sockaddr*)&addr, sizeof(addr));  // 连接服务器
    
//     write(sock_fd, "hello", 5);    // 发数据
//     char buf[100];
//     read(sock_fd, buf, 100);        // 收数据
    
//     close(sock_fd);
//     return 0;
// }