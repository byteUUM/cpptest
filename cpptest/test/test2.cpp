#include<iostream>
using namespace std;
// class A
// {   
// public:
//     void Print()
//     {
//         cout << "A::Print()" << endl;
//     }
// private:
//     int _a;
// };
// int main()
// {
//     A* p = nullptr;
//     p->Print();
//     return 0;
// }
class A
{ 
public:
    void Print()
    {
        cout << "A::Print()" << endl;
        cout << _a << endl;
    }
private:
    int _a;
};
int main()
{
    A* p = nullptr;
    p->Print();
    return 0;
}