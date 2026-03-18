#include <iostream>
#include "BitMap.hpp"
#include <ctime>
using namespace std;

// 测试函数
void TestBitMap() {
    cout << "========== 位图测试开始 ==========" << endl;
    
    // 测试1：基本功能测试
    cout << "\n【测试1】基本功能测试" << endl;
    BitMap bm(100);  // 支持0-100范围内的数
    
    // 测试set和find
    bm.set(5);
    bm.set(10);
    bm.set(50);
    bm.set(99);
    
    cout << "设置数据：5, 10, 50, 99" << endl;
    cout << "查找5：" << (bm.find(5) ? "存在" : "不存在") << endl;
    cout << "查找10：" << (bm.find(10) ? "存在" : "不存在") << endl;
    cout << "查找50：" << (bm.find(50) ? "存在" : "不存在") << endl;
    cout << "查找99：" << (bm.find(99) ? "存在" : "不存在") << endl;
    cout << "查找30：" << (bm.find(30) ? "存在" : "不存在") << endl;
    
    // 测试erase
    cout << "\n删除数据50" << endl;
    bm.erase(50);
    cout << "查找50：" << (bm.find(50) ? "存在" : "不存在") << endl;
    
    // 测试2：边界测试
    cout << "\n【测试2】边界测试" << endl;
    BitMap bm2(32);  // 刚好一个int
    
    bm2.set(0);
    bm2.set(31);
    
    cout << "设置数据：0, 31" << endl;
    cout << "查找0：" << (bm2.find(0) ? "存在" : "不存在") << endl;
    cout << "查找31：" << (bm2.find(31) ? "存在" : "不存在") << endl;
    
    // 测试3：重复设置测试
    cout << "\n【测试3】重复设置测试" << endl;
    BitMap bm3(64);
    
    bm3.set(20);
    cout << "第一次设置20，查找20：" << (bm3.find(20) ? "存在" : "不存在") << endl;
    
    bm3.set(20);  // 重复设置
    cout << "第二次设置20，查找20：" << (bm3.find(20) ? "存在" : "不存在") << endl;
    
    bm3.erase(20);
    cout << "删除20后，查找20：" << (bm3.find(20) ? "存在" : "不存在") << endl;
    
    bm3.erase(20);  // 重复删除
    cout << "再次删除20后，查找20：" << (bm3.find(20) ? "存在" : "不存在") << endl;
    
    // 测试4：大量数据测试
    cout << "\n【测试4】大量数据测试" << endl;
    BitMap bm4(1000000);  // 支持0-999999的数
    const int TEST_COUNT = 10000;
    int success_count = 0;
    
    // 记录开始时间
    clock_t start = clock();
    
    // 设置测试数据
    for (int i = 0; i < TEST_COUNT; i++) {
        bm4.set(i * 100);  // 设置0,100,200,...
    }
    
    // 验证数据
    for (int i = 0; i < TEST_COUNT; i++) {
        if (bm4.find(i * 100)) {
            success_count++;
        }
    }
    
    clock_t end = clock();
    
    cout << "设置了" << TEST_COUNT << "个数据" << endl;
    cout << "成功验证：" << success_count << "个" << endl;
    cout << "耗时：" << (double)(end - start) * 1000 / CLOCKS_PER_SEC << "ms" << endl;
    
    // 测试5：交替设置和删除测试
    cout << "\n【测试5】交替设置和删除测试" << endl;
    BitMap bm5(1000);
    
    for (int i = 0; i < 10; i++) {
        int num = i * 50;
        bm5.set(num);
        cout << "设置" << num << "，查找结果：" << bm5.find(num) << " ";
        
        bm5.erase(num);
        cout << "删除" << num << "，查找结果：" << bm5.find(num) << endl;
    }
    
    // 测试6：随机操作测试
    cout << "\n【测试6】随机操作测试" << endl;
    BitMap bm6(1000);
    srand(time(nullptr));
    
    for (int i = 0; i < 20; i++) {
        int num = rand() % 1000;
        int op = rand() % 3;  // 0:set, 1:find, 2:erase
        
        switch(op) {
            case 0:
                bm6.set(num);
                cout << "设置 " << num << " -> 现在查找结果：" << bm6.find(num) << endl;
                break;
            case 1:
                cout << "查找 " << num << " -> 结果：" << bm6.find(num) << endl;
                break;
            case 2:
                bm6.erase(num);
                cout << "删除 " << num << " -> 现在查找结果：" << bm6.find(num) << endl;
                break;
        }
    }
    
    cout << "\n========== 位图测试结束 ==========" << endl;
}

int main() {
    TestBitMap();
    return 0;
}