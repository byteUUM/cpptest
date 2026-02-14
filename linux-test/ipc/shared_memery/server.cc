#include <iostream>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <unistd.h>
int main()
{
    int key = ftok(".",44);
    int shmid = shmget(key,4096,IPC_CREAT|IPC_EXCL|0660);
    void* p=shmat(shmid,nullptr,0);
    int* ps=(int*)p;
    for(int i=0;i<200;i++)
    {
        *ps=i;
        ps++;
        sleep(2);
    }
    return 0;
}