#include <iostream>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
int main()
{
    int key = ftok(".",44);
    int shmid = shmget(key,4096,IPC_CREAT);
    void* p=shmat(shmid,nullptr,0);
    int* ps=(int*)p;
    while(1)
    {
        std::cout<<*ps<<" "<<std::endl;
        ps++;
        sleep(1);
    }
    return 0;
}