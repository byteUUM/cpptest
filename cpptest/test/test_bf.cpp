#include<iostream>
#include<bitset>
using namespace std;

#include"BitSet.h"

//int main()
//{
//	std::bitset<100> bs;
//	cout << sizeof(bs) << endl;
//
//	bs.set(32);
//	bs.set(33);
//
//	bs.reset(33);
//	bs.set(34);
//
//	cout << bs.test(31) << endl;
//	cout << bs.test(32) << endl;
//	cout << bs.test(33) << endl;
//	cout << bs.test(34) << endl;
//	cout << bs.test(35) << endl;
//
//	std::bitset<-1>* ptr = new std::bitset<-1>();
//	//bit::bitset<0xffffffff> bs2;
//	//bit::bitset<UINT_MAX> bs2;
//
//	return 0;
//}

#include"BloomFilter.h"

int main()
{
	//test_bitset1();
	//test_twobitset();

	TestBloomFilter2();
}