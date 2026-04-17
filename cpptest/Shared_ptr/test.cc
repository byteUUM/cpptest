// #include "shared_ptr4.hpp"
// #include "shared_weak.hpp"
#include "shared_ptr6.hpp"
#include "test.hpp"
using namespace std;
struct mm
{
    int x;
    int y;
};
#if 0
void test1()
{
    sptr::my_shared_ptr<int> p1 = new int(666);
    sptr::my_shared_ptr<int> p2(p1);
    sptr::my_shared_ptr<int> p3(p1);
    sptr::my_shared_ptr<int> p4 = new int(777);
    sptr::my_shared_ptr<int> p5 = p4;

    cout<<*p2<<" "<<*p5<<endl;
    sptr::my_shared_ptr<mm> p6 = new mm({1,2});
    cout<<p6->x<<" "<<p6->y<<endl;
    return;
}
void test2() {
    auto* raw = new int(42);
    shared_ptr<int> sp1(raw);
    shared_ptr<int> sp2 = sp1;
    std::cout << *sp1 << " count=" << sp1.use_count() << "\n"; // 42 count=2

    weak_ptr<int> wp(sp1);
    std::cout << "expired=" << wp.expired() << "\n";           // 0

    sp1 = shared_ptr<int>(); sp2 = shared_ptr<int>();          // 释放强引用
    std::cout << "expired=" << wp.expired() << "\n";           // 1
    std::cout << "lock valid=" << (bool)wp.lock() << "\n";     // 0
}
void test3() {
    using namespace swptr;
    auto* raw = new int(42);
    my_shared_ptr<int> sp1(raw);
    my_shared_ptr<int> sp2 = sp1;
    std::cout << *sp1 << " count=" << sp1.use_count() << "\n"; // 42 count=2

    my_weak_ptr<int> wp(sp1);
    std::cout << "expired=" << wp.expired() << "\n";           // 0

    sp1 = my_shared_ptr<int>(); sp2 = my_shared_ptr<int>();          // 释放强引用
    std::cout << "expired=" << wp.expired() << "\n";           // 1
    std::cout << "lock valid=" << (bool)wp.lock() << "\n";     // 0
}
#endif
void test()
{
    sptr::my_shared_ptr<int> p1(new int(666));
    sptr::my_shared_ptr<int> p2(p1);
    sptr::my_shared_ptr<int> p3(p1);
    sptr::my_shared_ptr<int> p4(new int(777));
    sptr::my_shared_ptr<int> p5 = p4;

    
    sptr::my_shared_ptr<int> p6(new int(888));
    sptr::my_shared_ptr<int> p7(move(p6));
    cout<<*p2<<" "<<*p5<<endl;
    sptr::my_shared_ptr<mm> p8(new mm({1,2}));
    cout<<p8->x<<" "<<p8->y<<endl;
    return;
}
int main()
{
    test();
    return 0;
}