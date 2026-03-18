// #pragma once
// #include <cstdio>
// #include <iostream>
// void _quick_sort(int* arr,int left,int right)
// {
//     if(left>=right) return;
//     int target = arr[(left+right)/2];
//     int l=left-1,r=right+1;
//     for(int i=left;i<r;i++)
//     {
//         if(arr[i]<target) swap(arr[i],arr[++l]);
//         else if(arr[i]>target) swap(arr[i--],arr[--r]);
//     }
//     _quick_sort(arr,left,l);
//     _quick_sort(arr,r,right);
// }
// void quick_sort(int* arr,int sz)
// {
//     _quick_sort(arr,0,sz-1);
// }
#pragma once
#include <cstdio>
#include <iostream>
void _quick_sort(int* arr,int left,int right)
{
    if(left>=right) return;
    int target = arr[(left+right)/2];
    int l=left-1,r=right+1,i=left;
    while(i<r)
    {
        if(arr[i]<target) swap(arr[++l],arr[i++]);
        else if(arr[i]>target) swap(arr[--r],arr[i]);
        else i++;
    }
    _quick_sort(arr,left,l);
    _quick_sort(arr,r,right);
}
void quick_sort(int* arr,int sz)
{
    _quick_sort(arr,0,sz-1);
}