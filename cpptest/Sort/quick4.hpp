#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

void quick_sort(std::vector<int>& arr, int left, int right)
{
    srand((unsigned int)time(nullptr));
    if(left>=right) return;
    int key = arr[left+rand()%(right-left+1)];
    int l = left-1, r = right+1, cur = left;
    while(cur<r)
    {
        if(arr[cur]==key) cur++;
        else if(arr[cur]<key) std::swap(arr[++l],arr[cur++]);
        else std::swap(arr[--r],arr[cur]);
    }
    quick_sort(arr, left, l);
    quick_sort(arr, r, right);
}