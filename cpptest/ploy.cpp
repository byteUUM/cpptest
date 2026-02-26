#include<iostream>
using namespace std;
class ticket
{
	public:
			virtual void func()
					{
								cout << "普通票" << endl;
									}
	private:
};
class student:public ticket
{
	public:
			virtual void func()
					{
								cout << "学生票" << endl;
									}
	private:
};
void fm(ticket& pu)
{
		pu.func();
}
int main()
{
		ticket tk;
			student stu;
				fm(tk);
					fm(stu);
						return 0;
}
