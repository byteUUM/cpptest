#include <iostream>
#include <thread>
using namespace std;
int x = 0;
void Print(int n)
{
    for(int i=0;i<n;i++)
    {
        x++;
    }
    return;
}
int main()
{
    thread t1(Print,100000);
    thread t2(Print,200000);
    t1.join();
    t2.join();
    cout<<x<<endl;
    return 0;
}