#include <iostream>
using namespace std;
#include <vector>
int main()
{
	vector<int> v{ 1,2,3,4,5,6 };
	auto it = v.begin();
	// v.resize(100, 8);
	// 将有效元素个数增加到100个，多出的位置使用8填充，操作期间底层会扩容
	
	// v.reserve(100);
	// reserve的作用就是改变扩容大小但不改变有效元素个数，操作期间可能会引起底层容量改变
	
	// v.insert(v.begin(), 0);
	// v.push_back(8);
	// 插入元素期间，可能会引起扩容，而导致原空间被释放
	// 给vector重新赋值，可能会引起底层容量改变
	v.assign(100, 8);
	/*
	出错原因：以上操作，都有可能会导致vector扩容，也就是说vector底层原理旧空间被释
	放掉，而在打印时，it还使用的是释放之间的旧空间，在对it迭代器操作时，实际操作的是一块
	已经被释放的空间，而引起代码运行时崩溃。
	解决方式：在以上操作完成之后，如果想要继续通过迭代器操作vector中的元素，只需给
	it重新赋值即可。
	*/
	while (it != v.end())
	{
		cout << *it << " ";
		++it;
	} 
	cout << endl;
	return 0;
}
