#include "server.hpp"

using namespace epoll_demo_model;
int main()
{
    TcpServer ts;
    ts.Start();
    sleep(60000);
    ts.Close();
    return 0;
}