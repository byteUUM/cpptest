#pragma once
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
using namespace std;
class InetAddr
{
public:
    InetAddr(){}
    InetAddr(sockaddr_in &peer)
        : _addr(peer)
    {
        _port = ntohs(peer.sin_port);
        _ip = inet_ntoa(peer.sin_addr);
    }
    InetAddr(uint16_t port, string ip)
        : _port(port), _ip(ip)
    {
        _addr.sin_family = AF_INET;
        _addr.sin_port = htons(_port);
        _addr.sin_addr.s_addr = inet_addr(_ip.c_str());
    }
    string tostring_port()
    {
        return to_string(_port);
    }
    string tostring_ip()
    {
        return _ip;
    }
    bool operator==(InetAddr addr)
    {
        return _port == addr._port && _ip == addr._ip;
    }
    sockaddr_in &getaddr()
    {
        return _addr;
    }
 
private:
    uint16_t _port;
    string _ip;
    sockaddr_in _addr;
};