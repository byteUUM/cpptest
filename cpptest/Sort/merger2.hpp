#pragma once 
#include <iostream>
#include <vector>
#include <functional>

//priorit_queue的比较函数怎么包装的
void merger_sort(std::vector<int>& arr, int left, int right)
{
    if(left>=right) return;
    int mid = (left+right)/2;
    merger_sort(arr,left,mid);
    merger_sort(arr,mid+1,right);
    int l = left, r = mid+1;
    static std::vector<int> tmp(arr.size());
    int pos = left;
    while(l<=mid&&r<=right)
    {
        if(arr[l]<arr[r]) tmp[pos++] = arr[l++];
        else tmp[pos++] = arr[r++];
    }
    while(l<=mid) tmp[pos++] = arr[l++];
    while(r<=right) tmp[pos++] = arr[r++];
    --pos;
    while(pos>=left) arr[pos--] = tmp[pos];
}