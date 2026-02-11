#include <iostream>
#include <cstdio>
#include <sys/wait.h>
#include <unistd.h>
#include <cstring>
#include <sys/stat.h>
#include <fcntl.h>
using namespace std;
#if 0
int main()
{
    FILE* f = fopen("./text.txt","w");
    if(!f)
    {
        cout<<"file open fail"<<endl;
        return 0;
    }
    char* argv[]={"ls -al",NULL};
    char buffer[1024];
    int count = 0;
    if(!fork())
    {
        //execl("usr/bin/ls -al",argv);
        for(int i='A';i<'z';i++)
        {
            //char c=i;
            sprintf(buffer+count,"%c ",i);
            count+=2;
            // fwrite(&c,sizeof(char),1,f);
        }
        buffer[count++]='\n';
        buffer[count] = '\0';
        fwrite(buffer,sizeof(char),strlen(buffer),f);
        exit(0);
    }
    int state;
    int ret = waitpid(-1,&state,0);
    if((state&0x7F)==0)
    {
        cout<<"程序正常退出 exit code: "<<((state>>8)&0xFF)<<endl;
    }
    else
    {
        cout<<"sig code: "<<(state&0x7F)<<endl;
    }
    fclose(f);
    return 0;
}

int main()
{
    FILE* f = fopen("./text.txt","r");
    if(!f)
    {
        cout<<"file open fail"<<endl;
        return 0;
    }
    char* argv[]={"ls -al",NULL};
    char buffer[1024];
    int count = 0;
    if(!fork())
    {
        while(fread(buffer+count,1,sizeof(char),f))
        {
            count++;
        }
        cout<<buffer<<endl;
        exit(0);
    }
    int state;
    int ret = waitpid(-1,&state,0);
    if((state&0x7F)==0)
    {
        cout<<"程序正常退出 exit code: "<<((state>>8)&0xFF)<<endl;
    }
    else
    {
        cout<<"sig code: "<<(state&0x7F)<<endl;
    }
    fclose(f);
    return 0;
}
int main()
{
    const char* s="hello,linux-ubuntu!\n";
    fprintf(stdout,"%s",s);
    FILE* f = fopen("out.txt","w");
    fprintf(f,"%s",s);
    fclose(f);
    return 0;
}
//linux文件操作系统接口
//#endif
int main()
{
    int fd = open("text2.txt",O_WRONLY|O_CREAT,660);
    if(fd<0)
    {
        cout<<"open fail!"<<endl;
    }
    const char* s = "hello open!\n";
    write(fd,s,strlen(s));
    close(fd);
    return 0;
}
int main()
{
    int fd = open("text2.txt",O_RDONLY);
    if(fd<0)
    {
        cout<<"open fail!"<<endl;
    }
    char buffer[1024];
    //while(ret = read(fd,buffer,sizeof(buffer)-1)>0);
    int ret = read(fd,buffer,sizeof(buffer)-1)>0;
    //buffer[ret] = '\0';
    fwrite(buffer,sizeof(char),strlen(buffer),stdout);
    write(1,buffer,strlen(buffer));
    //cout<<buffer<<endl;
    close(fd);
    return 0;
}

//fd分配规则测试
int main()
{
    close(1);
    int fd = open("shell.cc",O_RDONLY);
    int fd2 = open("text.txt",O_WRONLY|O_APPEND);
    char c = 48+fd;
    char c2 = 48+fd2;
    write(fd2,&c,1);
    write(fd2,&c2,1);
    close(fd);
    close(fd2);
    return 0;
}

//fd重定向测试
int main()
{
    int fd = open("/dev/null",O_WRONLY);
    dup2(fd,1);
    for(int i=0;i<10;i++)
        cout<<2*i-1<<endl;
    return 0;
}

//缓冲区以及刷新机制验证

//子进程？
//umask码占几字节？
int main()
{

    close(1);
    int fd = open("text3.txt",O_WRONLY|O_CREAT|O_TRUNC,0660);
    printf("fddasfda\n");
    //stderr（2号文件描述符）没有缓冲区验证：
    dup2(fd,2);
    perror("hello error\n");
    close(fd);
    return 0;

}

//验证程序替换使用的是子进程的文件描述符表
int main()
{
    if(!fork())
    {
        close(1);
        int fd = open("text4.txt",O_WRONLY|O_CREAT|O_TRUNC,0660);
        if(fd<0) exit(-1);
        execlp("ls","ls",NULL);
        exit(0);
    }
    int state;
    int ret = waitpid(-1,&state,0);
    if(ret<0)
    {
        cout<<"waitpid fail!"<<endl;
    }
    //fflush(stdout);
    return 0;
}


int main()
{
    //stdout默认是行刷新所以直接输出三个字符串
    //如果重定向到某文件中，会变成全刷新，而fork()会刷新一次，主进程本身也会刷新一次所以输出五个字符，
    //其中write是系统接口，数据没保留到缓冲区，所以只输出了一次。
    const char *msg0="hello printf\n";
    const char *msg1="hello fwrite\n";
    const char *msg2="hello write\n";
    printf("%s", msg0);
    fwrite(msg1, strlen(msg0), 1, stdout);
    write(1, msg2, strlen(msg2));
    fork();
    return 0;
}
//libc设计思想

//组织结构，必须要有fd，缓冲区，刷新刷新策略
//1. 与系统调用最大的区别，增加了用户缓冲区
//2. 不同点二，刷新策略（和动态设置），到达/n时调用write把它刷新到内核文件缓冲区，再使用fsync刷新到外设

#endif
int main()
{

    close(1);
    int fd = open("text3.txt",O_WRONLY|O_CREAT|O_TRUNC,0660);
    printf("fddasfda\n");
    //stderr（2号文件描述符）没有缓冲区验证：
    dup2(fd,2);
    perror("hello error\n");
    close(fd);
    return 0;
}

