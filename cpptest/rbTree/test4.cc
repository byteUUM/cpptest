#include "rbTree5.hpp"
#include <iostream>
#include <cstdlib>
using namespace std;
using namespace rbTree_model;

void test()
{
    srand((unsigned int)time(nullptr));
    rbTree<int> tree;
    for(int i=0;i<100;i++)
    {
        tree.Insert(rand()%1000+i);
    }
    cout<<tree.IsRBTree()<<endl;
}
int main()
{
    test();
    return 0;
}