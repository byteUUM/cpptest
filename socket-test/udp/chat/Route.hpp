#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Log.hpp"
#include "InetAddr.hpp"
using namespace std;
using namespace my_log;
class Route
{
private:
    void Push(InetAddr peer)
    {
        for (auto val : _data)
        {
            if (val == peer) return;
        }
        _data.push_back(peer);
        LOG(Level::INFO)<<peer.tostring_ip()<<'|'<<peer.tostring_port()<<" online";
    }
    bool Pop()
    {
        return true;
    }
 
public:
    Route(){}
    void Handler(int socketfd,string message,InetAddr client)
    {
        Push(client);
        // 处理信息
        string send_message = client.tostring_ip() + " | " + client.tostring_port() + ": ";
        send_message += message;
        // 发给所有在线的客户端
        for (auto val : _data)
        {
            if(val == client) continue;
            sendto(socketfd, send_message.c_str(), send_message.size(), 0, (sockaddr *)&val.getaddr(), sizeof(val.getaddr()));
        }
    }
private:
    vector<InetAddr> _data;
};