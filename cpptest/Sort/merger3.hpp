#pragma once
#include <iostream>
#include <vector>
using namespace std;

void merger_sort(vector<int>& arr,int left, int right)
{
    if(left>=right) return;
    int mid = left+(right-left)/2;
    merger_sort(arr,left,mid);
    merger_sort(arr,mid+1,right);
    static vector<int> tmp(arr);
    int pos = left;
    int p1=left, p2=mid+1;
    while(p1<=mid&&p2<=right)
    {
        if(arr[p1]<arr[p2]) tmp[pos++] = arr[p1++];
        else tmp[pos++] = arr[p2++];
    }
    while(p1<=mid) tmp[pos++] = arr[p1++];
    while(p2<=right) tmp[pos++] = arr[p2++];
    for(int i=left;i<=right;i++) arr[i] = tmp[i];
}