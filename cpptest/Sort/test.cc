// // #include "shell.hpp"
// // #include "quick.hpp"
// // #include "merger.hpp"
// // #include "heap.hpp"
// #include "shell2.hpp"
// #include <stdlib.h>
// int main()
// {
//     srand((unsigned int)time(nullptr));
//     int arr[100];
//     for(int i=0;i<100;i++) arr[i] = rand()%1000+i;
//     shell_sort(arr,100);
//     for(int i=0;i<100;i++) cout<<*(arr+i)<<" ";
//     cout<<endl;
// }
#if 0
#include "heap2.hpp"
using namespace std;
int main()
{
    srand((unsigned int)time(nullptr));
    vector<int> arr(100,0);
    for(int i=0;i<100;i++) arr[i] = rand()%1000+i;
    heap_sort(arr);
    for(auto x:arr) cout<<x<<" ";
    cout<<endl;
}
#endif

#include "merger2.hpp"
using namespace std;
int main()
{
    srand((unsigned int)time(nullptr));
    vector<int> arr(100,0);
    for(int i=0;i<100;i++) arr[i] = rand()%1000+i;
    merger_sort(arr,0,arr.size()-1);
    for(auto x:arr) cout<<x<<" ";
    cout<<endl;
}
