#pragma once
#include <vector>

class BitMap
{
public:
    //构造函数
    BitMap(size_t num)
    {
        //num除以32计算需要的多少个4字节空间
        _bites.resize((num>>5)+1,0);
    }
    //把数据标为1
    void set(size_t val)
    {
        int index = val/32;
        int pos = val%32;
        _bites[index]|=(1<<pos);
    }
    //把数据标记为0
    void erase(size_t val)
    {
        int index = val/32;
        int pos = val%32;
        _bites[index]&=(~(1<<pos));
    }
    //查看数据是否存在
    bool find(size_t val)
    {
        int index = val/32;
        int pos = val%32;
        return (_bites[index]>>pos)&1;
    }
private:
    std::vector<int> _bites;
};