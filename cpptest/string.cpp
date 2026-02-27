​
#define _CRT_SECURE_NO_WARNINGS 1
#include<iostream>
using namespace std;
namespace byte
{
	class string
	{
	public:
		typedef char* iterator;
		friend ostream& operator<<(ostream& out, string& s);
		friend istream& operator>>(istream& put, string& s);
	public:
		string(const char* st = "")
		{
			size_t sz = strlen(st);
			_str = new char[sz + 1];
			strcpy(_str, st);
			_size = sz;
			_capacity = sz;
		}
		void swap(string& str)
		{
			std::swap(_str, str._str);
			std::swap(_capacity, str._capacity);
			std::swap(_size, str._size);
		}
		string(const string& st)
		{
			string sv(st._str);
			swap(sv);
		}
		string operator=(string& st)
		{
			swap(st);
			return *this;
		}
		~string()
		{
			delete[] _str;
			_str = nullptr;
			_capacity = _size = 0;
		}
		size_t size()
		{
			return _size;
		}
		size_t capacity()
		{
			return _capacity;
		}
		typedef char* iterator;
		iterator begin()
		{
			return _str;
		}
		iterator end()
		{
			return _str + _size;
		}
		iterator cbegin() const
		{
			return _str;
		}
		iterator cend() const
		{
			return _str + _size;
		}
		char operator[](size_t index)
		{
			return _str[index];
		}
		bool operator<(const string& s)
		{
			return strcmp(_str, s._str) < 0;
		}
		bool operator==(const string& s)
		{
			return strcmp(_str, s._str) == 0;
		}
		bool operator<=(const string& s)
		{
			return *this < s || *this == s;
		}
		bool operator>(const string& s)
		{
			return !(*this < s || *this == s);
		}
		bool operator>=(const string& s)
		{
			return !(*this < s);
		}
		bool operator!=(const string& s)
		{
			return !(*this == s);
		}
		void resize(size_t n, char m = '?')
		{
			if (n <= _size)
				return;
			while (_size < n)
			{
				_str[_size++] = m;
			}
		}
		void reserve(size_t n)
		{
			if (n < _size)
				return;
			char* st = new char[n + 1];
			strcpy(st, _str);
			delete[] _str;
 
			_str = st;
			_capacity = n;
		}
		void clear()
		{
			_size = 0;
		}
		void push_back(char c);
		string& operator+=(char c);
		void append(const char* str);
		string& operator+=(const char* str);
		iterator insert(size_t pos, char c);
		iterator erase(size_t pos);
	private:
		char* _str;
		size_t _capacity;
		size_t _size;
	};
}
namespace byte
{
	void string::push_back(char c)
	{
		if (_size == _capacity)
			reserve(_capacity == 0 ? 4 : 2 * _capacity);
		_str[_size++] = c;
		_str[_size] = '\0';
	}
	void string::append(const char* str)
	{
		reserve(_size + strlen(str) + 1);//提前开空间减少扩容带来的效率损耗
		for (int i = 0; str[i] != '\0'; i++)
		{
			push_back(str[i]);
		}
	}
	string& string::operator+=(char c)
	{
		push_back(c);
		return *this;
	}
	string& string::operator+=(const char* str)
	{
		append(str);
		return *this;
	}
	ostream& operator<<(ostream& out, string& s)
	{
		for (auto vul : s)
		{
			out << vul;
		}
		return out;
	}
	istream& operator>>(istream& put, string& s)
	{
		s.clear();
		const size_t N = 256;
		char arr[N];
		char c;
		c = put.get();
		int i = 0;
		while (c != '\n' && c != ' ')
		{
			if (i != N - 1)
			{
				arr[i++] = c;
			}
			else
			{
				arr[i++] = '\0';
				s += arr;
				i = 0;
			}
			c = put.get();
		}
		if (i != 0)
		{
			arr[i] = '\0';
			s += arr;
		}
		return put;
	}
	string::iterator string::insert(size_t pos, char c)
	{
		if (_size == _capacity)
			reserve(_capacity == 0 ? 4 : _capacity * 2);
		int end = _size;
		while (end != pos - 1)
		{
			_str[end + 1] = _str[end];
			end--;
		}
		_str[pos] = c;
		_size++;
		return begin() + pos + 1;
	}
	string::iterator string::erase(size_t pos)
	{
		int bin = pos;
		while (bin != _size)
		{
			_str[bin] = _str[bin + 1];
			bin++;
		}
		_size--;
		return begin() + pos;
	}
}
namespace byte
{
	void string_test1()
	{
		string x("123456");
		cout << x << endl;
		string k(x);
		cout << k << endl;
		string mstr;
		cin >> mstr;
		for (auto n : mstr)
		{
			cout << n;
		}
	}
	void string_test2()
	{
		//reserve
		string str("zxcvbnm");
		cout << "capacity:" << str.capacity() << endl;
 
		str.reserve(20);
		cout << "capacity:" << str.capacity() << endl;
 
		str.reserve(15);
		cout << "capacity:" << str.capacity() << endl;
 
		str.reserve(3);
		cout << "capacity:" << str.capacity() << endl;
 
		//resize
		str.resize(10, '0');
		cout << "size:" << str.size() << ' ' << str << endl;
 
		str.resize(3, '0');
		cout << "size:" << str.size() << ' ' << str << endl;
	}
	void string_test3()
	{
		string s("123456");
 
		s.push_back('x');
		cout << s << endl;
 
		s.append("vvv");
		cout << s << endl;
 
		s += "hhh";
		cout << s << endl;
	}
 
	void string_test4()
	{
		string s("12345678");
 
		s.erase(5);
		cout << s << endl;
 
		s.erase(2);
		cout << s << endl;
 
		s.insert(0,'0');
		cout << s << endl;
	}
}
int main()
{
	byte::string_test1();
	//byte::string_test2();
	//byte::string_test3();
	//byte::string_test4();
	return 0;
}
