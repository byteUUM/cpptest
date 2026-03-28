#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>
using namespace std;
void quick_sort(vector<int>& arr,int l,int r)
{
    if(l>=r) return;
    int key = arr[l+rand()%(r-l)];
    int left = l-1, right = r+1, cur = l;
    while(cur<right)
    {
        if(arr[cur]<key) swap(arr[++left],arr[cur++]);
        else if(arr[cur]>key) swap(arr[--right],arr[cur]);
        else cur++;
    }
    quick_sort(arr,l,left);
    quick_sort(arr,right,r);
}
