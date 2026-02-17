#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <strings.h>
#include <arpa/inet.h>
#include <thread>
#include "InetAddr.hpp"
#include "Log.hpp"
using namespace my_log;
void Write(int socketfd, InetAddr &addr)
{
    string str="online";
    sendto(socketfd, str.c_str(), sizeof(str), 0, (const sockaddr *)&addr.getaddr(), sizeof(addr.getaddr()));
    while (true)
    {
        std::string message;
        cout<<"Please Enter# ";
        std::getline(std::cin, message);
        sendto(socketfd, message.c_str(), sizeof(message), 0, (const sockaddr *)&addr.getaddr(), sizeof(addr.getaddr()));
    }
}
void Read(int socketfd)
{
    while (true)
    {
        sockaddr_in sd;
        char buffer[1024];
        socklen_t len = sizeof(sd);
        int n = recvfrom(socketfd, buffer, sizeof(buffer) - 1, 0, (sockaddr *)&sd, &len);
        buffer[n] = '\0';
        std::cout << buffer << std::endl;
    }
}
 
int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " server_op server_port" << std::endl;
        return 1;
    }
    int socketfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socketfd < 0)
    {
        LOG(Level::ERROR) << "socket() fail";
        exit(1);
    }
    else
    {
        LOG(Level::INFO) << "socket() succee _socketfd:" << socketfd;
    }
    InetAddr addr((uint16_t)std::stoi(argv[2]), argv[1]);
    thread td_read([&](){
        Write(socketfd,addr);
    });
    thread td_write([&](){
        Read(socketfd);
    });
    td_read.join();
    td_read.join();
    return 0;
}