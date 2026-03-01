#pragma once
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <iostream>
#include "task.hpp"

class Channel
{
public:
    Channel(int wfd, int id)
    :_wfd(wfd),_id(id)
    {}
    void Write(int code)
    {
        write(_wfd,&code,sizeof(int));
    }
    void Close()
    {
        close(_wfd);
    }
    void Wait()
    {
        waitpid(_id,NULL,0);
    }
private:
    int _wfd;
    int _id;
};

class ChannelManage
{
public:
    ChannelManage()
    :_next(0)
    {}
    int get_next()
    {  
        int ret = _next;
        _next++;
        _next%=_channels.size();
        return ret;
    } 
    void Write(int channel_code, int task_code)
    {
        _channels[channel_code].Write(task_code);
    }
    void Close()
    {
        for(auto cln:_channels)
        {
            cln.Close();
        }
    }
    void Wait()
    {
        for(auto cln:_channels)
        {
            cln.Wait();
        }
    }
    void Insert(int fd, int id)
    {
        _channels.push_back(Channel(fd,id));
    }
    private:
    vector<Channel> _channels;
    int _next;
};

class ProcessPool
{
public:
    ProcessPool(int n,Task task)
    :_n(n),_tasks(task)
    {}
    void work(int rfd)
    {
        while(true)
        {
            int code;//任务号
            int ret = read(rfd,&code,sizeof(int));
            if(ret == 0)
            {
                cout<<"main fd close!"<<endl;
                return;
            }
            else if(ret == -1)
            {
                cout<<"read fail!"<<endl;
                return;
            }
            _tasks.running(code);
        }
    }
    void CreateProcess()
    {
        for(int i=0;i<_n;i++)
        {
            int fp[2];
            int ret = pipe(fp);
            if(ret == -1)
            {
                cout<<"pipe fail"<<endl;
                exit(-1);
            }
            int pid = fork();
            if(pid==-1)
            {
                cout<<"create process fail！"<<endl;
                exit(-2);
                //return;
            }
            if(!pid)
            {
                close(fp[1]);
                work(fp[0]);
                close(fp[0]);
                exit(0);
            }
            close(fp[0]);
            _cm.Insert(fp[1],pid);
        }
    }
    int Run()
    {
        //获取管道号
        int channel_code = _cm.get_next();
        //获取任务号
        int task_code = _tasks.get_code();
        _cm.Write(channel_code,task_code);
    }
    void Stop()
    {
        _cm.Close();
        _cm.Wait();
    }
private:
    ChannelManage _cm;
    Task _tasks;
    int _n;
};