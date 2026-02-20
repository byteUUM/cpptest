#pragma once
#include<string>
#include"BitSet.h"

struct HashFuncBKDR
{
	// @detail 本 算法由于在Brian Kernighan与Dennis Ritchie的《The CProgramming Language》
	// 一书被展示而得 名，是一种简单快捷的hash算法，也是Java目前采用的字符串的Hash算法累乘因子为31。
	size_t operator()(const std::string& s)
	{
		size_t hash = 0;
		for (auto ch : s)
		{
			hash *= 31;
			hash += ch;
		}
		return hash;
	}
};


struct HashFuncAP
{
	// 由Arash Partow发明的一种hash算法。  
	size_t operator()(const std::string& s)
	{
		size_t hash = 0;
		for (size_t i = 0; i < s.size(); i++)
		{
			if ((i & 1) == 0) // 偶数位字符
			{
				hash ^= ((hash << 7) ^ (s[i]) ^ (hash >> 3));
			}
			else              // 奇数位字符
			{
				hash ^= (~((hash << 11) ^ (s[i]) ^ (hash >> 5)));
			}
		}

		return hash;
	}
};

struct HashFuncDJB
{
	// 由Daniel J. Bernstein教授发明的一种hash算法。 
	size_t operator()(const std::string& s)
	{
		size_t hash = 5381;
		for (auto ch : s)
		{
			hash = hash * 33 ^ ch;
		}

		return hash;
	}
};

template<size_t N,
	size_t X = 5,
	class K = std::string,
	class Hash1 = HashFuncBKDR,
	class Hash2 = HashFuncAP,
	class Hash3 = HashFuncDJB>
class BloomFilter
{
public:
	void Set(const K& key)
	{
		size_t hash1 = Hash1()(key) % M;
		size_t hash2 = Hash2()(key) % M;
		size_t hash3 = Hash3()(key) % M;
		//cout << hash1 <<" "<< hash2 <<" "<< hash3 << endl;

		_bs.set(hash1);
		_bs.set(hash2);
		_bs.set(hash3);
	}

	bool Test(const K& key)
	{
		size_t hash1 = Hash1()(key) % M;
		if (!_bs.test(hash1))
		{
			return false;
		}

		size_t hash2 = Hash2()(key) % M;
		if (!_bs.test(hash2))
		{
			return false;
		}

		size_t hash3 = Hash3()(key) % M;
		if (!_bs.test(hash3))
		{
			return false;
		}
		
		return true; // 可能存在误判
	}

	// 获取公式计算出的误判率
	double getFalseProbability()
	{
		double p = pow((1.0 - pow(2.71, -3.0 / X)), 3.0);

		return p;
	}


private:
	static const size_t M = N * X;
	bit::bitset<M> _bs;
};

void TestBloomFilter1()
{
	BloomFilter<10> bf;
	bf.Set("猪八戒");
	bf.Set("孙悟空");
	bf.Set("唐僧");

	cout << bf.Test("猪八戒") << endl;
	cout << bf.Test("孙悟空") << endl;
	cout << bf.Test("唐僧") << endl;
	cout << bf.Test("沙僧") << endl;
	cout << bf.Test("猪八戒1") << endl;
	cout << bf.Test("猪戒八") << endl;
}

void TestBloomFilter2()
{
	srand(time(0));
	const size_t N = 1000000;
	BloomFilter<N> bf;
	//BloomFilter<N, 3> bf;
	//BloomFilter<N, 10> bf;

	std::vector<std::string> v1;
	//std::string url = "https://www.cnblogs.com/-clq/archive/2012/05/31/2528153.html";
	//std::string url = "https://www.baidu.com/s?ie=utf-8&f=8&rsv_bp=1&rsv_idx=1&tn=65081411_1_oem_dg&wd=ln2&fenlei=256&rsv_pq=0x8d9962630072789f&rsv_t=ceda1rulSdBxDLjBdX4484KaopD%2BzBFgV1uZn4271RV0PonRFJm0i5xAJ%2FDo&rqlang=en&rsv_enter=1&rsv_dl=ib&rsv_sug3=3&rsv_sug1=2&rsv_sug7=100&rsv_sug2=0&rsv_btype=i&inputT=330&rsv_sug4=2535";
	std::string url = "猪八戒";

	for (size_t i = 0; i < N; ++i)
	{
		v1.push_back(url + std::to_string(i));
	}

	for (auto& str : v1)
	{
		bf.Set(str);
	}

	// v2跟v1是相似字符串集（前缀一样），但是后缀不一样
	v1.clear();
	for (size_t i = 0; i < N; ++i)
	{
		std::string urlstr = url;
		urlstr += std::to_string(9999999 + i);
		v1.push_back(urlstr);
	}

	size_t n2 = 0;
	for (auto& str : v1)
	{
		if (bf.Test(str)) // 误判
		{
			++n2;
		}
	}
	cout << "相似字符串误判率:" << (double)n2 / (double)N << endl;

	// 不相似字符串集  前缀后缀都不一样
	v1.clear();
	for (size_t i = 0; i < N; ++i)
	{
		//string url = "zhihu.com";
		string url = "孙悟空";
		url += std::to_string(i + rand());
		v1.push_back(url);
	}

	size_t n3 = 0;
	for (auto& str : v1)
	{
		if (bf.Test(str))
		{
			++n3;
		}
	}
	cout << "不相似字符串误判率:" << (double)n3 / (double)N << endl;

	cout << "公式计算出的误判率:" << bf.getFalseProbability() << endl;
}