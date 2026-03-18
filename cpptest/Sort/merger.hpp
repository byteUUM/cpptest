#include <iostream>
#include <vector>
int* tmp = NULL;
void _merger_sort(int* arr,int begin,int end)
{
    if(begin>=end) return;
    int mid = (begin+end)/2;
    _merger_sort(arr,begin,mid);
    _merger_sort(arr,mid+1,end);
    int l = begin, r = mid+1;
    int pos = begin;
    while(l<=mid&&r<=end)
    {
        if(arr[l]<arr[r]) tmp[pos++] = arr[l++];
        else tmp[pos++] = arr[r++];
    }
    while(l<=mid) tmp[pos++] = arr[l++];
    while(r<=end) tmp[pos++] = arr[r++];
    for(int i = begin;i<=end;i++) arr[i]=tmp[i];
}
void merger_sort(int* arr,int sz)
{
    tmp = (int*)malloc(sz * sizeof(int));
    _merger_sort(arr,0,sz-1);
    free(tmp);
}