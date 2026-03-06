#include <iostream>
#include "rbTree1.hpp"
#include <cstdio>
using namespace std;

int main()
{
    srand((unsigned int)time(nullptr));
    rbTree<int> tree;
    for(int i=0;i<200;i++)
    {
        int val = rand()%1000;
            tree.insert(val);
        cout<<val<<" ";
        if((i+1)%10==0) cout<<endl;
    }
    cout<<endl;
    cout<<tree.IsRBTree()<<endl;
    return 0;
}