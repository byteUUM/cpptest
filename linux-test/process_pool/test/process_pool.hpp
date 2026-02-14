#include <unistd.h>
#include <fcntl.h>
#include <iostream>
#include <sys/wait.h>
#include <vector>
#include "Task.hpp"
class Channel
{
public:
    Channel(int wfd,int pid)
    :_wfd(wfd),_pid(pid)
    {}
    void write(int code)
    {
        ::write(_wfd,&code,sizeof(code));
    }
    void close()
    {
        ::close(_wfd);
    }
    void wait()
    {
        waitpid(_pid,NULL,0);
    }
private:
    int _wfd;
    int _pid;    
};
class ChannelManage
{
public:
    void Insert(int wfd,int pid)
    {
        _channels.push_back(Channel(wfd,pid));
    }
    void write(int channel_code, int task_code)
    {
        _channels[channel_code].write(task_code);
    }
    int get_code()
    {
        int ret = _next++;
        _next%=_channels.size();
        return ret;
    }
    void close()
    {
        for(auto cnl:_channels)
        {
            cnl.close();
        }
    }
    void wait()
    {
        for(auto cnl:_channels)
        {
            cnl.wait();
        }
    }

private:
    vector<Channel> _channels;
    int _next;
};

class Process_Pool
{
private:
    void work(int rfd)
    {
        int code;
        int ret = read(rfd,&code,sizeof(int));
        if(ret==0)
        {
            return;
        }
        else if(ret<0)
        {
            cout<<"read fail"<<endl;
            return;
        }
        _tks.work(code);
    }
public:
    Process_Pool(int n, Tasks ts)
    :_n(n),_tks(ts){}
    void Create()
    {
        for(int i=0;i<_n;i++)
        {
            int pfd[2];
            pipe(pfd);
            int pid = fork();
            if(!pid)
            {
                close(pfd[1]);
                work(pfd[0]);
                exit(0);
            }
            close(pfd[0]);
            _cm.Insert(pfd[1],pid);
        }
    }
    void Run()
    {
        int channel_code=_cm.get_code();
        int task_code = _tks.get_code();
        _cm.write(channel_code,task_code);
    }
    void Stop()
    {
        _cm.close();
        _cm.wait();
    }
private:
    ChannelManage _cm;
    Tasks _tks;
    int _n;
};