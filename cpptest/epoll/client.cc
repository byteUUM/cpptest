#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

void is_error(int x, std::string s)
{
    if(x==-1)
    {
        std::cout<<s<<" fail"<<std::endl;
        exit(-1);
    }
}
int main()
{
    int cfd = socket(AF_INET, SOCK_STREAM, 0);
    is_error(cfd,"socket");

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
    int ret = connect(cfd, (struct sockaddr*)&addr, sizeof(addr));
    is_error(ret,"connect");

    while(true)
    {
        std::cout<<"输出消息>>";
        std::string message;
        std::getline(std::cin, message);
        
        send(cfd, message.c_str(), message.size(),0);
        char buffer[1024];
        int n = recv(cfd,buffer,sizeof(buffer)-1,0);
        if(n==0) return 0;
        if(n<0) is_error(-1,"recv");
        else
        {
            buffer[n] = '\0';
            std::cout<<"server say: "<<buffer<<std::endl;
        }
    }
    return 0;
}