#include<iostream>
using namespace std;
// class A
// { 
// public:
//     A(int a)
//     :_a1(a), _a2(_a1)
//     {}
//     void Print() {
//         cout << _a1 << " " << _a2 << endl;
//     }
// private:
//     int _a2 = 2;
//     int _a1 = 2;
// };
// int main()
// {
//     A aa(1);
//     aa.Print();
// }
class Sum
{ 
public:
    Sum()
    {
        _ret += _i;
        ++_i;
    }
    static int GetRet()
    {
        return _ret;
    }
    private:
    static int _i;
    static int _ret;
};
int Sum::_i = 1;
int Sum::_ret = 0;
class Solution 
{
public:
    int Sum_Solution(int n) {
    // 变⻓数组
    Sum arr[n];
    return Sum::GetRet();
    }
};
int main()
{
    Solution s;
    cout<<s.Sum_Solution(2)<<endl;
    return 0;
}