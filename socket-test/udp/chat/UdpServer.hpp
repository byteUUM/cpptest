// 用条件编译，防止头文件重复包含
#ifndef UDP_SERVER
#define UDP_SERVER
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <strings.h>
#include <string>
#include <assert.h>
#include <arpa/inet.h>
#include <functional>
#include "InetAddr.hpp"
using namespace std;
using funcType = function<void(int, string, InetAddr)>;
class task
{
public:
    task() {}
    task(funcType func, int socketfd, string message, InetAddr client)
        : _func(func), _socketfd(socketfd), _message(message), _client(client)
    {
    }
    void operator()()
    {
        assert(_socketfd != -1);
        _func(_socketfd, _message, _client);
    }
 
private:
    funcType _func;
    int _socketfd;
    string _message;
    InetAddr _client;
};
class UdpServer
{
public:
    UdpServer(uint16_t port, const funcType &func)
        : _socketfd(-1), _port(port), _func(func)
    {
    }
    void Init()
    {
        // 打开网络文件 IPv4 数据包 udp
        _socketfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (_socketfd < 0)
        {
            std::cout << "socket() fail"<<endl;
            exit(1);
        }
        else
        {
            std::cout << "socket() succee _socketfd:" << _socketfd<<endl;
        }
        sockaddr_in sd;
        bzero(&sd, sizeof(sd));
        sd.sin_family = AF_INET;
        sd.sin_port = htons(_port);
        // sd.sin_addr.s_addr = inet_addr(_ip.c_str());
        sd.sin_addr.s_addr = INADDR_ANY;
 
        //  绑定ip地址与端口号
        int n = bind(_socketfd, (const sockaddr *)&sd, sizeof(sd));
        if (n < 0)
        {
            std::cout << "bind fial"<<std::endl;
            exit(1);
        }
        else
        {
            std::cout << "bind success"<<std::endl;
        }
 
    }
    void Start()
    {
        while (true)
        {
            // 收各个客户端发来的消息
            sockaddr_in client;
            socklen_t len = sizeof(client);
            char buffer[1024];
            int n = recvfrom(_socketfd, buffer, sizeof(buffer), 0, (sockaddr *)&client, &len);
            buffer[n] = '\0';
 
            _func(_socketfd,string(buffer),InetAddr(client));
 
        }
    }
    ~UdpServer()
    {
    }
 
private:
    int _socketfd;
    uint16_t _port;
    funcType _func;
};
#endif