#include <iostream>
#include <cstdio>
using namespace std;
void shell_sort(int* arr,int sz)
{
    int group = sz;
    while(group>1)
    {
        group = group/3 + 1;
        for(int i=0;i<sz-group;i++)
        {
            int j = i;
            int tmp = arr[j+group];
            while(j>=0&&tmp<arr[j])
            {
                arr[j+group] = arr[j];
                j-=group;
            }
            arr[j+group] = tmp;
        }
    }
}