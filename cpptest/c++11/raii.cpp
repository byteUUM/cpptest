#include <iostream>
#include <string>
#include <memory>
using namespace std;

class person
{
public:
enum Gender{MAN,WOMAN};
    //void skill() = 0;
    virtual void order_type()
    {
        cout<<"普通票"<<endl;
    }
private:
    string _name;
    int _age;
    Gender _gender;
    string _identity;
};
class student:virtual public person
{
    virtual void order_type() override
    {
        cout<<"学生票"<<endl;
    }
};
class soldier:virtual public person
{
    virtual void order_type() override
    {
        cout<<"优待票"<<endl;
    }
};

class always:public student, public soldier
{
    virtual void order_type() override final
    {
        cout<<"always"<<endl;
    }
};
void test1()
{
    shared_ptr<person> conwy(make_shared<person>());
    conwy->order_type();
    shared_ptr<person> zhangsan(make_shared<student>());
    zhangsan->order_type();

    shared_ptr<person> lish(make_shared<soldier>());
    lish->order_type();

    shared_ptr<person> wangwu(make_shared<always>());
    wangwu->order_type();
};
void test2()
{
    auto func = [](FILE* f){fclose(f);};
    unique_ptr<FILE,decltype(func)> fptr(fopen("file.txt","r"),func);
    //抛异常
}
int main()
{
    test2();
    return 0;
}