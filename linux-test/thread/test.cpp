#include <pthread.h>
#include <iostream>
#include <unistd.h>
using namespace std;
void* fun(void* x)
{
    int* p = (int*)x;
    for(int i=0;i<*p;i++)
    {
        sleep(1);
        cout<<i<<" "<<endl;
    }
    cout<<endl;
    return nullptr;
}
int main()
{
    pthread_t tid;
    int x = 10;
    pthread_create(&tid,nullptr,fun,&x);
    //for(char c='A';c<'z';c++) cout<<c<<endl;
    //pthread_detach(tid);
    sleep(5);
    pthread_cancel(tid);
    //pthread_join(tid,nullptr);
    while(true) sleep(1);
    return 0;
}