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
    int listenfd = socket(AF_INET,SOCK_STREAM,0);
    if(listenfd<0)
    {
        cout<<"socket fail!"<<endl;
        exit(1);
    }
    struct sockaddr_in sn={AF_INET,htons(9002),INADDR_ANY};
    int ret = bind(listenfd,(struct sockaddr*)&sn,sizeof(sn));
    if(ret<0)
    {
        cout<<"bind fail!"<<endl;
        exit(2);
    }
    listen(listenfd,5);
    //阻塞获取fd
    int sockfd = accept(listenfd,nullptr,nullptr);//后两个为输出型参数客户端sockaddr和它的长度
    if(sockfd<0)
    {
        cout<<"accpet fail!"<<endl;
        exit(3);
    }
    char buffer[1024];
    while(true)
    {
        int ret = read(sockfd,buffer,sizeof(buffer)-1);
        if(ret<=0)
        {
            cout<<"read fail"<<endl;
            exit(4);
        }
        buffer[ret]=0;
        cout<<buffer<<endl;
    }
    close(listenfd);
    close(sockfd);
    return 0;
}


// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <unistd.h>

// int main() {
//     // 1个socket监听，1个socket通信
//     int listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    
//     struct sockaddr_in addr = {AF_INET, htons(8888), INADDR_ANY};
//     bind(listen_fd, (struct sockaddr*)&addr, sizeof(addr));
    
//     listen(listen_fd, 5);  // 开始监听
    
//     int conn_fd = accept(listen_fd, NULL, NULL);  // 接受连接
    
//     char buf[100];
//     read(conn_fd, buf, 100);      // 读数据
//     write(conn_fd, "world", 5);   // 写数据
    
//     close(conn_fd);
//     close(listen_fd);
//     return 0;
// }