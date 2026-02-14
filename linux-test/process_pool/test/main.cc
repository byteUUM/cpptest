#include "process_pool.hpp"
void test1()
{
    cout<<"test1"<<endl;
}
void test2()
{
    cout<<"test2"<<endl;
}
void test3()
{
    cout<<"test3"<<endl;
}
int main()
{
    Tasks ts;
    ts.Insert(test1);
    ts.Insert(test2);
    ts.Insert(test3);
    Process_Pool pp(10,ts);
    pp.Create();
    pp.Run();
    pp.Stop();
    return 0;
}