#include "shared_ptr4.hpp"
using namespace std;
struct mm
{
    int x;
    int y;
};
int main()
{
    sptr::my_shared_ptr<int> p1 = new int(666);
    sptr::my_shared_ptr<int> p2(p1);
    sptr::my_shared_ptr<int> p3(p1);
    sptr::my_shared_ptr<int> p4 = new int(777);
    sptr::my_shared_ptr<int> p5 = p4;

    cout<<*p2<<" "<<*p5<<endl;
    sptr::my_shared_ptr<mm> p6 = new mm({1,2});
    cout<<p6->x<<" "<<p6->y<<endl;
    return 0;
}