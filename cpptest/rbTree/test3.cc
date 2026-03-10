#include "rbTree3.hpp"
#include <cstdlib>
using namespace std;
void test()
{
    srand((unsigned int)time(nullptr));
    rbTree<int> tree;
    for(int i=0;i<100;i++)
    {
        tree.insert(rand()%1000+i);
    }
    cout<<tree.IsRBTree()<<endl;
}
int main()
{
    test();
    return 0;
}