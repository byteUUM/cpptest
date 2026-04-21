#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <thread>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <vector>
#include <sys/epoll.h>
#include <fcntl.h>
#include <assert.h>
namespace epoll_demo_model
{
    class TcpServer
    {
    private:
        const int NUM = 10;
        const int MAXEVENTS = 128
        void is_error(int x, std::string s)
        {
            if(x == -1)
            {
                std::cout<<s<<" fail"<<std::endl;
                if(s!="accept") exit(-1);
            }
        }
        void Read(int fd)
        {
            while(true)
            {
                char buffer[1024];
                int ret = recv(fd, buffer, sizeof(buffer)-1,0);
                if(ret == 0) return;
                else if(ret<0)
                {
                    //没数据->break
                    //信号中断->continue
                    //异常->异常处理->return
                }
                else buffer[ret] = '\0';
                std::string message = buffer;
                message = "receive "+message;
                send(fd, message.c_str(),message.size(),0);
            }
        }
        void Write(int fd)
        {

        }
        void Accept(int fd)
        {
            int fdt = fcntl(fd,F_GETFD);
            fcntl(fd,F_SETFD,fdt|O_NOBLOCK);
            epoll_event evt;
            evt.data.fd = fd;
            evt.events = EPOLLIN|EPOLLOUT|EPOLLET;
            int ret = epoll_ctl(_efd, EPOLL_CTL_ADD, fd, &evt);
            if(ret == -1)
            {
                std::cout<<fd<<" epoll_ctl fail";
                return;
            }
        }
    public:
        int Start()
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
            //1.创建epoll模型
            int efd = epoll_create(0);
            //sfd设置为非阻塞
            //获取sfd控制信息
            int sft = fcntl(sfd, F_GETFD);
            //添加标准位为非阻塞
            fcntl(sfd, F_SETFD, sft|O_NOBLOCK);

            //2.将sfd监听描述符让epoll模型管理
            //2.1 组织信息
            epoll_event evt;
            evt.data.fd = sfd;
            evt.events = EPOLLIN|EPOLLET;//设置为读事件监控和ET模式
            //2.2 设置event信息
            int ret = epoll_ctl(efd, EPOLL_CTL_ADD, sfd, &evt);
            assert(ret!=-1);

            //开始监听
            ret = listen(sfd, NUM);
            assert(ret!=-1);

            //获取连接和收发消息
            //管理连接
            std::vector<std::thread> _threads;
            while(true)
            {
                int n = epoll_wait(_efd, &events, MAXEVENTS, 1000);
                if(n==0)
                {
                    std::cout<<"timeout..."<<endl;
                }
                else if(n<0)
                {
                    error(-1,"epoll_wait fail");
                }
                else
                {
                    for(int i = 0; i<n; i++)
                    {
                        uint32_t events = events[i].events;
                        int fd = events[i].data.fd;
                        if(event&EPOLLERR)
                        {
                            //异常处理
                        }
                        if(events&EPOLLIN)
                        {
                            if(fd == _sfd) Accept(fd);
                            else Read(fd);
                        }
                        if(events&EPOLLOUT)
                        {
                            Write(fd);
                        }

                    }
                }
            }
            while(!_threads.empty())
            {
                _threads.back().join();
                _threads.pop_back();
            }
            close(sfd);
            return 0;
        }
    private:
        int _sfd = -1;
        int _efd = -1;
        int open
        epoll_event _events[MAXEVENTS];
    };
}
