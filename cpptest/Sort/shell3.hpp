#pragma once 
#include <iostream>
#include <vector>
using namespace std;

void shell_sort(vector<int>& arr)
{
    int group = arr.size();
    while(group>1)
    {
        group = group/3+1;
        for(int i=group;i<arr.size();i++)
        {
            int tmp = arr[i];
            int j = i;
            while(j-group>=0&&tmp<arr[j-group])
            {
                arr[j] = arr[j-group];
                j-=group;
            }
            arr[j] = tmp;
        }
    }
}