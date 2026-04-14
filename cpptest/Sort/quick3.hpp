// #include <iostream>
// #include <vector>
// #include <cstdlib>
// #include <cstdio>

// void quick_sort(std::vector<int>& arr, int left, int right)
// {
//     if(left>=right) return;
//     int key = arr[left+rand()%(right-left)];
//     int l = left-1, r = right+1, pos = left;
//     while(pos<r)
//     {
//         if(arr[pos] == key) ++pos;
//         else if(arr[pos]<key) std::swap(arr[++l], arr[pos++]);
//         else std::swap(arr[--r],arr[pos]);
//     }
//     quick_sort(arr,left,l);
//     quick_sort(arr,r,right);
// }

// #include <iostream>
// #include <vector>
// #include <cstdlib>
// #include <cstdio>

// void quick_sort(std::vector<int>& arr, int l, int r)
// {
//     if(l>=r) return;
//     int key = arr[l+rand()%(r-l)];
//     int left = l-1, right = r+1, cur = l;
//     while(cur < right)
//     {
//         if(arr[cur]<key) std::swap(arr[++left], arr[cur++]);
//         else if(arr[cur]>key) std::swap(arr[--right], arr[cur]);
//         else ++cur;
//     }
//     quick_sort(arr, l, left);
//     quick_sort(arr, right, r);
// }

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdio>

void quick_sort(std::vector<int>& arr, int l, int r)
{
    if(l>=r) return;
    srand((unsigned int)time(nullptr));
    int key = arr[l + rand()%(r-l)];
    int left = l-1, right = r+1, cur = l;
    while(cur < right)
    {
        if(arr[cur]<key) std::swap(arr[++left], arr[cur++]);
        else if(arr[cur]>key) std::swap(arr[--right], arr[cur]);
        else cur++;
    }
    quick_sort(arr, l, left);
    quick_sort(arr, right, r);
}