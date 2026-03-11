#include "HashTable.hpp"
using namespace std;
using namespace HashTable_Model;

void test()
{
    HashTable<int,string> ht;

    // 插入
    ht.Insert({1,"aaa"});
    ht.Insert({2,"bbb"});
    ht.Insert({3,"ccc"});

    // 查找
    auto node = ht.Find(2);
    if(node)
        cout<<"Find:"<<node->_kv.first<<" "<<node->_kv.second<<endl;

    // operator[]
    ht[4] = "ddd";
    cout<<ht[4]<<endl;

    // 删除
    ht.Erase(3);

    if(ht.Find(3) == nullptr)
        cout<<"erase success"<<endl;
}
int main()
{
    test();
    return 0;
}