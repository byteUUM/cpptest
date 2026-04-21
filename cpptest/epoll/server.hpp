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
#include <queue>
#include <unistd.h>
const int NUM = 10;
const int MAXEVENTS = 128;
namespace epoll_demo_model
{
    using namespace std;
    class TcpServer
    {
    private:
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
                if(ret == 0)
                {
                    //客户端退出
                    return;
                }
                else if(ret<0)
                {
                    //没数据->break;
                    if((errno==EAGAIN)||(errno==EWOULDBLOCK))
                    {
                        break;
                    }
                    else if(errno==EINTR)
                    {
                        //信号中断->continue
                        continue;
                    }
                    else
                    {
                        //异常->异常处理，return 
                    }
                }
                else buffer[ret] = '\0';
                std::string message = buffer;
                _buffer.push(message);
            }
        }
        void Write(int fd)
        {
            while(true)
            {
                string message;
                if(!_buffer.empty())
                {
                    message = _buffer.front();
                    _buffer.pop();
                    message = "server say: "+message;
                }
                else break;
                int ret = send(fd, message.c_str(),message.size(),0);
                if(ret == 0)
                {
                    //操作成功，但发送的字节为0
                    break;
                }
                else if(ret<0)
                {
                    //缓冲区满->break;
                    if((errno==EAGAIN)||errno==EWOULDBLOCK)
                    {
                        break;
                    }
                    else if(errno==EINTR)
                    {
                        //信号中断->continue
                        continue;
                    }
                    else
                    {
                        //异常->异常处理，return 
                    }
                }
                //发送成功
            }
        }
        void Accept()
        {
            while(true)
            {
                
                int fd = accept(_sfd,nullptr,nullptr);
                if(fd<0)
                {
                    //没数据->break;
                    if((errno==EAGAIN)||(errno==EWOULDBLOCK))
                    {
                        break;
                    }
                    else if(errno==EINTR)
                    {
                        //信号中断->continue
                        continue;
                    }
                    else
                    {
                        //异常->异常处理，return 
                    }
                }
                else
                {
                    int fdt = fcntl(fd,F_GETFL);
                    fcntl(fd,F_SETFL,fdt|O_NONBLOCK);
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
            }
        }
    public:
        int Start()
        {
            //创建socket文件
            _sfd = socket(AF_INET, SOCK_STREAM, 0);
            is_error(_sfd,"socket");
            sockaddr_in addr{};
            addr.sin_family = AF_INET;
            addr.sin_port = htons(8080);
            addr.sin_addr.s_addr = INADDR_ANY; 

            //绑定端口
            int ret = bind(_sfd, (struct sockaddr*)&addr, sizeof(addr));
            is_error(ret,"bind");

            //监听端口
            //1.创建epoll模型
            _efd = epoll_create(1);
            assert(_efd!=-1);
            //_sfd设置为非阻塞
            //获取_sfd控制信息
            int sft = fcntl(_sfd, F_GETFL);
            //添加标准位为非阻塞
            fcntl(_sfd, F_SETFL, sft|O_NONBLOCK);

            //2.将_sfd监听描述符让epoll模型管理
            //2.1 组织信息
            epoll_event evt;
            evt.data.fd = _sfd;
            evt.events = EPOLLIN|EPOLLET;//设置为读事件监控和ET模式
            //2.2 设置event信息
            ret = epoll_ctl(_efd, EPOLL_CTL_ADD, _sfd, &evt);
            assert(ret!=-1);

            //开始监听
            ret = listen(_sfd, NUM);
            assert(ret!=-1);

            //获取连接和收发消息
            //管理连接
            //std::vector<std::thread> _threads;
            _is_open = true;
            while(_is_open)
            {
                int n = epoll_wait(_efd, _events, MAXEVENTS, 1000);
                if(n==0)
                {
                    std::cout<<"timeout..."<<endl;
                }
                else if(n<0)
                {
                    is_error(-1,"epoll_wait fail");
                }
                else
                {
                    for(int i = 0; i<n; i++)
                    {
                        uint32_t events = _events[i].events;
                        int fd = _events[i].data.fd;
                        if(events&EPOLLERR)
                        {
                            //异常处理
                        }
                        if(events&EPOLLIN)
                        {
                            if(fd == _sfd) Accept();
                            else Read(fd);
                        }
                        if(events&EPOLLOUT)
                        {
                            Write(fd);
                        }

                    }
                }
            }
            // while(!_threads.empty())
            // {
            //     _threads.back().join();
            //     _threads.pop_back();
            // }
            close(_sfd);
            close(_efd);
            return 0;
        }
        void Close()
        {
            _is_open = false;
        }
    private:
        int _sfd = -1;
        int _efd = -1;
        bool _is_open = false;
        std::queue<std::string> _buffer;
        epoll_event _events[MAXEVENTS];
    };
}
