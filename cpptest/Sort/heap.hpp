#pragma once
#include <iostream>
void up_heap(int* arr,int x)
{
    int child = x;
    int parent = (x-1)/2;
    while(child>0)
    {
        if(arr[child]>arr[parent]) swap(arr[child],arr[parent]);
        else break;
        child = parent;
        parent = (child-1)/2;
    }
}
void down_heap(int* arr,int x)
{
    swap(arr[0],arr[x]);
    int parent = 0;
    int child = parent*2+1;
    while(child<x)
    {
        if(child+1<x&&arr[child+1]>arr[child]) child++;
        if(arr[child]>arr[parent]) swap(arr[child],arr[parent]);
        parent = child;
        child = parent*2+1;
    }
}
void heap_sort(int* arr, int sz)
{
    //升序，建大堆
    for(int i=0;i<sz;i++) up_heap(arr,i);
    //把堆顶元素，交换到数组尾
    for(int i=sz-1;i>0;i--) down_heap(arr,i);
}