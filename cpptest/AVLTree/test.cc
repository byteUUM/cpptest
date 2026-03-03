#include <iostream>
#include <cstdio>
#include "AVLTree1.hpp"

void test()
{
    srand((unsigned int)time(nullptr));
    AVLTree<int> tree;
    int i=100;
    while(i--)
    {
        tree.insert(rand()%1000);
    };
    if(tree.IsAVLTree())
    std::cout<<"true"<<std::endl;
    else std::cout<<"false"<<std::endl;
}
int main()
{
    test();
    return 0;
}