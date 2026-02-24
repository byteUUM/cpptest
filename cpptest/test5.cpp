#include<iostream>
#include<string>
#include<functional>
#include<memory>
using namespace std;
template<class T>
class My_share_ptr
{
public:
	My_share_ptr(T* ptr, function<void(T*)> det= [](T* p) {delete p; })
		:_ptr(ptr),_count(new int(1)),_det(det){}
	My_share_ptr(const My_share_ptr& sptr)
	{
		_ptr = sptr._ptr;
		_count = sptr._count;
		_det = sptr._det;
		(*_count)++;
	}
	My_share_ptr& operator=(const My_share_ptr& sptr)
	{
		if (sptr._ptr == _ptr) return *this;
		if (--(*_count) == 0)
		{
			_det(_ptr);
			delete _count;
			cout << "Delete1()" << endl;
		}
		_ptr = sptr._ptr;
		_count = sptr._count;
		(*_count)++;
		return *this;
	}
	T& operator*()
	{
		return *_ptr;
	}
	T* operator->()
	{
		return _ptr;
	}
 
	~My_share_ptr()
	{
		if (--(*_count) == 0)
		{
			_det(_ptr);
			delete _count;
			cout << "Delete2()" << endl;
		}
	}
private:
	T* _ptr;
	int* _count;
	function<void(T*)> _det;
};
int main()
{
	//My_share_ptr<int> p(new int(8));
	//auto p2 = p;
	//My_share_ptr<int> c = p;
	My_share_ptr<int> p1(new int(8));
	My_share_ptr<int> p2(new int(7));
	My_share_ptr<int> p3=p1;
	p1 = p2;
	return 0;
}
