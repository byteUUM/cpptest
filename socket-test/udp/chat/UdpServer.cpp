#include <iostream>
#include <memory>
#include "UdpServer.hpp"
#include "InetAddr.hpp"
#include "Route.hpp"
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        // LOG(Level::FATAL)<<"input error";
        std::cerr << "Usage" << argv[0] << "port" << std::endl;
        return 1;
    }
    std::string port = argv[1];
    // 路由
    Route rt;
    // 通信
    unique_ptr<UdpServer> us = make_unique<UdpServer>(stoi(port), [&](int socketfd, string meassge, InetAddr client)
                                                      { rt.Handler(socketfd, meassge, client); });
    us->Init();
    us->Start();
    return 0;
}