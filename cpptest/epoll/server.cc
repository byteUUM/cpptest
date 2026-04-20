#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>

const int NUM = 10;

void is_error(int x, std::string s)
{
    if(x == -1)
    {
        std::cout<<s<<" fail"<<std::endl;
        if(s!="accept") exit(-1);
    }
}
void server(int fd, sockaddr_in* clientaddr)
{
    while(true)
    {
        char buffer[1024];
        int ret = recv(fd, buffer, sizeof(buffer)-1,0);
        if(ret == 0) return;
        else if(ret<0) is_error(-1,"recv");
        else buffer[ret] = '\0';
        std::string message = buffer;
        message = "receive "+message;
        send(fd, message.c_str(),message.size(),0);
    }
}
int main()
{
    //创建socket文件
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    is_error(sfd,"socket");
    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY; 

    //绑定端口
    int ret = bind(sfd, (struct sockaddr*)&addr, sizeof(addr));
    is_error(ret,"bind");

    //监听端口
    ret = listen(sfd, NUM);
    is_error(ret, "listen");

    //获取连接和收发消息
    //管理连接
    std::vector<std::thread> _threads;
    while(1)
    {
        sockaddr_in clientaddr;
        socklen_t len = sizeof(clientaddr);
        int cfd = accept(sfd, (struct sockaddr*)&clientaddr, &len);
        is_error(cfd, "accept");
        _threads.push_back(std::thread(server,cfd,&clientaddr));
    }
    while(!_threads.empty())
    {
        _threads.back().join();
        _threads.pop_back();
    }
    close(sfd);
    return 0;
}