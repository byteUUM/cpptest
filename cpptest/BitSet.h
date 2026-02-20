#pragma once
#include <vector>
#include <cmath>
namespace bit
{
	template<size_t N>
	class bitset
	{
	public:
		bitset()
		{
			_bs.resize(N / 32 + 1);
		}

		// x映射的位标记成1
		void set(size_t x)
		{
			size_t i = x / 32;
			size_t j = x % 32;

			_bs[i] |= (1 << j);
		}

		// x映射的位标记成0
		void reset(size_t x)
		{
			size_t i = x / 32;
			size_t j = x % 32;

			_bs[i] &= (~(1 << j));
		}

		// x映射的位是1返回真
		// x映射的位是0返回假
		bool test(size_t x)
		{
			size_t i = x / 32;
			size_t j = x % 32;

			return _bs[i] & (1 << j);
		}

	private:
		std::vector<int> _bs;
	};

	template<size_t N>
	class twobitset
	{
	public:
		void set(size_t x)
		{
			bool bit1 = _bs1.test(x);
			bool bit2 = _bs2.test(x);

			if (!bit1 && !bit2) // 00->01
			{
				_bs2.set(x);
			}
			else if (!bit1 && bit2) // 01->10
			{
				_bs1.set(x);
				_bs2.reset(x);
			}
			else if (bit1 && !bit2) // 10->11
			{
				_bs1.set(x);
				_bs2.set(x);
			}
		}

		// 返回0 出现0次数
		// 返回1 出现1次数
		// 返回2 出现2次数
		// 返回3 出现2次及以上
		int get_count(size_t x)
		{
			bool bit1 = _bs1.test(x);
			bool bit2 = _bs2.test(x);

			if (!bit1 && !bit2)
			{
				return 0;
			}
			else if (!bit1 && bit2)
			{
				return 1;
			}
			else if (bit1 && !bit2)
			{
				return 2;
			}
			else
			{
				return 3;
			}
		}

	private:
		bitset<N> _bs1;
		bitset<N> _bs2;
	};
};

void test_twobitset()
{
	bit::twobitset<100> tbs;
	int a[] = { 5,7,9,2,5,99,5,5,7,5,3,9,2,55,1,5,6,6,6,6,7,9 };
	for (auto e : a)
	{
		tbs.set(e);
	}

	for (size_t i = 0; i < 100; ++i)
	{
		//cout << i << "->" << tbs.get_count(i) << endl;
		if (tbs.get_count(i) == 1 || tbs.get_count(i) == 2)
		{
			cout << i << endl;
		}
	}
}

void test_bitset1()
{
	int a1[] = { 5,7,9,2,5,99,5,5,7,5,3,9,2,55,1,5,6 };
	int a2[] = { 5,3,5,99,6,99,33,66 };

	bitset<100> bs1;
	bitset<100> bs2;

	for (auto e : a1)
	{
		bs1.set(e);
	}

	for (auto e : a2)
	{
		bs2.set(e);
	}

	for (size_t i = 0; i < 100; i++)
	{
		if (bs1.test(i) && bs2.test(i))
		{
			cout << i << endl;
		}
	}
}
