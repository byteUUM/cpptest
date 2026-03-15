#include "BlockQueue3.hpp"
#include <vector>
#include <string>
#include <unistd.h>
using namespace std;
BlockQueue<string> qu;
void Input(string str)
{
    int count = 0;
    while(true)
    {
        string send = str+": " + to_string(count++);
        qu.Push(send);
    }
}
void Output()
{
    string ret;
    while(true)
    {
        sleep(1);
        ret = qu.Pop();
        cout<<ret<<endl;
    }
}
// int main()
// {
//     thread th1[3],th2[5];
//     for(int i=0;i<3;i++) th1[i] = thread(Input,string("th"+to_string(i)));
//     for(int i=0;i<5;i++) th2[i] = thread(Output);

//     for(int i=0;i<3;i++) th1[i].join();
//     for(int i=0;i<5;i++) th2[i].join();
//     return 0;
// }
int main()
{
    vector<thread> th1,th2;
    for(int i=0;i<3;i++) th1.emplace_back(Input,"th"+to_string(i));
    for(int i=0;i<5;i++) th2.emplace_back(Output);

    for(int i=0;i<3;i++) th1[i].join();
    for(int i=0;i<5;i++) th2[i].join();
    return 0;
}