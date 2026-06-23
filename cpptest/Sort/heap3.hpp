#pragma once
#include <iostream>
#include <vector>
using namespace std;

void up_create_heap(vector<int>& arr, int end)
{
    int child = end;
    int parent = (child-1)/2;
    while(child>0)
    {
        if(arr[child]>arr[parent])
            swap(arr[child],arr[parent]);
        else return;
        child = parent;
        parent = (child-1)/2;
    }
}
void down_create_heap(vector<int>& arr, int end)
{
    int parent = 0;
    int child = parent*2+1;
    while(child<end)
    {
        if((child+1)<end&&arr[child+1]>arr[child])
            child++;
        if(arr[child]>arr[parent])
            swap(arr[child],arr[parent]);
        else return;
        parent = child;
        child = parent*2+1;
    }
}
void heap_sort(vector<int>& arr)
{
    for(int i=0;i<arr.size();i++) up_create_heap(arr,i);
    int pos = arr.size()-1;
    while(pos>0)
    {
        swap(arr[pos],arr[0]);
        down_create_heap(arr,pos--);
    }
}
