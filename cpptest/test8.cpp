class Student
{ p
ublic:
//默认构造函数
Student()
{
num=1001;
age=18;
} /
/初始化构造函数
Student(int n,int a):num(n),age(a){}
private:
int num;
int age;
};
int main()
{
//用默认构造函数初始化对象S1
Student s1;
//用初始化构造函数初始化对象S2
Student s2(1002,18);
return 0;
}
class Test
{
int i;
int *p;
public:
Test(int ai,int value)
{
i = ai;
p = new int(value);
} ~
Test()
{
delete p;
} T
est(const Test& t)
{
this->i = t.i;
this->p = new int(*t.p);
}
};
//复制构造函数用于复制本类的对象
int main(int argc, char* argv[])
{
Test t1(1,2);
Test t2(t1);//将对象t1复制给t2。注意复制和赋值的概念不同
return 0;
}
