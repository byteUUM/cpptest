#include <iostream>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define PAGE_SIZE 4096
int main(int argc, char* argv[])
{
    if(argc!=2)
    {
        std::cout<<"Usage"<<argv[0]<<" filename"<<std::endl;
        return 1;
    }
    int fd = open(argv[1],O_RDWR|O_CREAT|O_TRUNC,0666);
    if(fd<0)
    {
        std::cout<<"file open fail"<<std::endl;
        return 2;
    }
    // 2. 调整文件大小
    if(::ftruncate(fd, PAGE_SIZE)==-1)
    {
        std::cout<<"fturncate fail"<<std::endl;
        return 3;
    }
    //3. 文件映射
    char* shmaddr = (char*)::mmap(NULL,PAGE_SIZE,PROT_READ|PROT_WRITE,MAP_SHARED,fd,0);
    if(shmaddr==MAP_FAILED)
    {
        std::cout<<"mmap fail"<<std::endl;
        return 4;
    }
    //4. 操作文件
    for(char c='a';c<='z';c++)
    {
        shmaddr[c-'a']=c;
        sleep(1);
    }
    //5. 关闭文件映射
    if(::munmap(shmaddr,PAGE_SIZE)==-1)
    {
        std::cout<<"munmap fail"<<std::endl;
        return 5;
    }
    ::close(fd);
    return 0;
}