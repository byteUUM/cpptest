#pragma once
#include <iostream>
#include <vector>
using namespace std;

void up_create(vector<int>& arr, int pos)
{
    int child = pos;
    int parent = (child-1)/2;
    while(child>0)
    {
        if(arr[child]>=arr[parent]) swap(arr[child],arr[parent]);
        else return;
        child = parent;
        parent = (child-1)/2;
    }
}
void down_create(vector<int>& arr, int pos)
{
    swap(arr[0],arr[pos]);
    int parent = 0;
    int child = parent*2+1;
    while(child<pos)
    {
        if(child+1<pos&&arr[child+1]>arr[child]) child++;
        if(arr[child]>=arr[parent]) swap(arr[child],arr[parent]);
        parent = child;
        child = parent*2+1;
    }
}
void heap_sort(vector<int>& arr)
{
    for(int i=0;i<arr.size();i++) up_create(arr,i);
    for(int i=arr.size()-1;i>=0;i--) down_create(arr,i);
}