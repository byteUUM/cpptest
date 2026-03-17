#include "shell.hpp"
#include <stdlib.h>
int main()
{
    srand((unsigned int)time(nullptr));
    int arr[100];
    for(int i=0;i<100;i++) arr[i] = rand()%1000+i;
    shell_sort(arr,100);
    for(int i=0;i<100;i++) cout<<*(arr+i)<<" ";
    cout<<endl;
}