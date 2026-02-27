#include<iostream>
#include<vector>
using namespace std;
namespace byte
{
	template<typename T>
	class less
	{
	public:
		//如果返回true则b优先，返回false则a优先，所以这里使用gt会生成小堆
		bool operator()(T a, T b)
		{
			return a < b;
		}
	private:
	};
	template<typename T>
	class greater
	{
	public:
		//如果返回true则b优先，返回false则a优先，所以这里使用gt会生成小堆
		bool operator()(T a, T b)
		{
			return a > b;
		}
	private:
	};
 
	template<typename T, class Container = vector<T>, class Compare = less<T>>
	class priority_queue
	{
	public:
 
		void AdjustUP()
		{
			int child = arr.size() - 1, father = (child - 1) / 2;
			while (child > 0)
			{
				if (comp(arr[father], arr[child]))
				{
					std::swap(arr[child], arr[father]);
					child = father;
					father = (child - 1) / 2;
				}
				else
				{
					break;
				}
			}
		}
		void AdjustDOWN()
		{
			int father = 0, child = father * 2 + 1;
			while (child < arr.size())
			{
				if (child + 1 < arr.size() && comp(arr[child], arr[child + 1]))
				{
					child++;
				}
				if (comp(arr[father], arr[child]))
				{
					std::swap(arr[child], arr[father]);
					father = child;
					child = father * 2 + 1;
				}
				else
				{
					break;
				}
			}
		}
		void push(T x)
		{
			arr.push_back(x);
			AdjustUP();
		}
		void pop()
		{
			std::swap(arr[0], arr[arr.size() - 1]);
			arr.pop_back();
			AdjustDOWN();
		}
		T top()
		{
			return arr[0];
		}
		size_t size()
		{
			return arr.size();
		}
		bool empty()
		{
			return arr.size() == 0;
		}
	private:
		Container arr;
		Compare comp;
	};
}
