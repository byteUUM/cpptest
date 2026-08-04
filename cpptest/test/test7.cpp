#include <iostream>

class Animal {
public:
    virtual void speak() {
        std::cout << "Animal speaks" << std::endl;
    }
    virtual ~Animal() {}
};

class Dog : public Animal {
public:
    void speak() override {
        std::cout << "Dog barks: Woof!" << std::endl;
    }
};

int main() {
    Animal* p = new Dog();

    // 调用基类的虚函数实现（绕过多态）
    p->Animal::speak();

    delete p;
    return 0;
}