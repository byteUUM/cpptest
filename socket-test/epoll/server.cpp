#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <thread>
#include <assert.h>
using namespace std;
#if 0
int main()
{
    int listen_fd = socket(AF_INET,SOCK_STREAM,0);
    //返回值检验
    struct sockaddr_in sn={AF_INET,htons(9003),INADDR_ANY};
    int ret = bind(listen_fd,(struct sockaddr*)&sn,sizeof(sn));
    listen(listen_fd,10);
    while(true)
    {
        int sock_fd = accept(listen_fd,nullptr,nullptr);
        thread t1([&](){
            char buffer[1024];
            int ret = read(sock_fd,buffer,sizeof(buffer)-1);
            if(ret<=0) return;
            buffer[ret]=0;
            cout<<"fd_"<<sock_fd<<":"<<buffer<<endl;
        });
        t1.detach();
    }
    return 0;
}
#endif

#define MAXEVENTS 20
int main()
{
    int listen_fd = socket(AF_INET,SOCK_STREAM,0);
    struct sockaddr_in sn = {AF_INET,htons(9003),INADDR_ANY};
    int ret = bind(listen_fd,(struct sockaddr*)&sn,sizeof(sn));
    listen(listen_fd,10);
    int epoll_fd = epoll_create1(0);
    if(epoll_fd<0)
    {
        cout<<epoll_fd<<endl;
        cout<<"epoll_create fail!"<<endl;
        exit(1);
    }
    epoll_event event;
    event.data.fd = listen_fd;
    event.events = EPOLLIN;
    ret = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &event);
    epoll_event events[MAXEVENTS];
    while(true)
    {
        int tm = 1000;
        int n = epoll_wait(epoll_fd,events,MAXEVENTS,tm);
        if(n>0)
        {
            for(int i=0;i<n;i++)
            {
                int fd = events[i].data.fd;
                //uint16_t
                auto evs = events[i].events;
                if(fd==listen_fd)
                {
                    cout<<"get listen_fd: "<<listen_fd<<endl;
                    struct sockaddr_in client;
                    socklen_t len;
                    int sock_fd = accept(listen_fd,(struct sockaddr*)&client,&len);
                    epoll_event event;
                    event.data.fd = sock_fd;
                    event.events = EPOLLIN;
                    epoll_ctl(epoll_fd,EPOLL_CTL_ADD,sock_fd,&event);
                }
                else
                {
                    if(evs&EPOLLERR)
                    {
                        // evs|=EPOLLIN;
                        // evs|=EPOLLOUT;
                    }
                    if(evs&EPOLLIN)
                    {
                        char buffer[1024];
                        int ret = read(fd,buffer,sizeof(buffer)-1);
                        if(ret>0)
                        {
                            buffer[ret]=0;
                            cout<<"fd_"<<fd<<": "<<buffer<<endl;
                        }
                        else if(ret==0)
                        {
                            cout<<"client "<<fd<<" out..."<<endl;
                            epoll_ctl(epoll_fd,EPOLL_CTL_DEL,fd,nullptr);
                            close(fd);
                        }
                        else
                        {
                            cout<<"read fail!"<<endl;
                            epoll_ctl(epoll_fd,EPOLL_CTL_DEL,fd,nullptr);
                            close(fd);
                        }
                    }
                    else if(evs&EPOLLOUT)
                    {
                        //读缓冲区不满时触发
                    }
                    else
                    {
                        assert(nullptr);
                    }
                }
            }
        }  
        else if(n==0)
        {
            //cout<<"overtime..."<<endl;
        }
        else
        {
            cout<<"epoll_wait fail!"<<endl;
        }
    }
    return 0;
}