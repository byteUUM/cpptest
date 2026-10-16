#include <iostream>
struct Base {
    Base()          { f(); }
    virtual ~Base() { f(); }
    virtual void f() { std::cout << "Base::f\n"; }
};
struct Derived : Base {
    Derived()       { f(); }
    ~Derived() override { f(); }
    void f() override { std::cout << "Derived::f\n"; }
};

struct B { virtual void f() { puts("B"); } };
struct D : B { private: void f() override { puts("D"); } };

//B* p = new D;
//p->f();         // OK,静态类型 B,f 是 public,运行时调到 D::f
		// D d; d.f();     // 编译错误,静态类型 D,f 是 private
int main() 
{
    Derived d;
    B* p = new D;
   p->f(); 
}
// Base::f      // Base() 里,vptr 指向 Base 的 vtable
// Derived::f   // Derived() 里
// Derived::f   // ~Derived() 里
// Base::f      // ~Base() 里,派生部分已销毁,vptr 退回 Base
