// #include <iostream>
// #include <string>
// #include "BloomFilter.hpp"  // 替换为实际文件名

// int main() 
// {
//     // 创建布隆过滤器：预计存储10个元素，每个元素占5位
//     BloomFilter<10> bf;
    
//     // 测试1：插入和查找
//     std::cout << "=== 测试插入和查找 ===" << std::endl;
//     bf.Set("apple");
//     bf.Set("banana");
//     bf.Set("orange");
    
//     std::cout << "apple: " << (bf.Find("apple") ? "存在" : "不存在") << std::endl;    // 应该存在
//     std::cout << "banana: " << (bf.Find("banana") ? "存在" : "不存在") << std::endl;  // 应该存在
//     std::cout << "orange: " << (bf.Find("orange") ? "存在" : "不存在") << std::endl;  // 应该存在
    
//     // 测试2：查找未插入的元素
//     std::cout << "\n=== 测试未插入的元素 ===" << std::endl;
//     std::cout << "grape: " << (bf.Find("grape") ? "存在" : "不存在") << std::endl;    // 大概率不存在
    
//     // 测试3：查看理论误判率
//     std::cout << "\n=== 理论误判率 ===" << std::endl;
//     std::cout << "误判率: " << bf.getFalseProbability() << std::endl;
    
//     return 0;
// }

#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <chrono>
#include <iomanip>
#include <bitset>

// 包含你的布隆过滤器头文件
#include "BloomFilter.hpp"

// 生成随机字符串
std::string randomString(int length) {
    static const char charset[] = 
        "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, sizeof(charset) - 2);
    
    std::string str;
    for (int i = 0; i < length; ++i) {
        str += charset[dis(gen)];
    }
    return str;
}

// 移到函数外部定义 - 标准版本
class BloomFilterWithCount {
private:
    static const size_t M = 10000 * 5;
    std::bitset<M> _bs;
    
public:
    void Set(const std::string& key) {
        size_t h1 = HashFuncBKDR()(key) % M;
        size_t h2 = HashFuncAP()(key) % M;
        size_t h3 = HashFuncDJB()(key) % M;
        
        _bs.set(h1);
        _bs.set(h2);
        _bs.set(h3);
    }
    
    bool Find(const std::string& key) {
        size_t h1 = HashFuncBKDR()(key) % M;
        if (!_bs.test(h1)) return false;
        
        size_t h2 = HashFuncAP()(key) % M;
        if (!_bs.test(h2)) return false;
        
        size_t h3 = HashFuncDJB()(key) % M;
        if (!_bs.test(h3)) return false;
        
        return true;
    }
    
    double getFillRatio() const { 
        return static_cast<double>(_bs.count()) / M; 
    }
};

// 移到函数外部定义 - 自定义版本
class BloomFilterCustom {
private:
    static const size_t M = 10000 * 5;
    std::bitset<M> _bs;
    
public:
    void Set(const std::string& key) {
        // 使用更多哈希函数
        size_t h1 = HashFuncBKDR()(key) % M;
        size_t h2 = HashFuncAP()(key) % M;
        size_t h3 = HashFuncDJB()(key) % M;
        size_t h4 = (h1 + h2) % M;  // 组合哈希
        size_t h5 = (h2 + h3) % M;  // 组合哈希
        
        _bs.set(h1);
        _bs.set(h2);
        _bs.set(h3);
        _bs.set(h4);
        _bs.set(h5);
    }
    
    bool Find(const std::string& key) {
        size_t h1 = HashFuncBKDR()(key) % M;
        if (!_bs.test(h1)) return false;
        
        size_t h2 = HashFuncAP()(key) % M;
        if (!_bs.test(h2)) return false;
        
        size_t h3 = HashFuncDJB()(key) % M;
        if (!_bs.test(h3)) return false;
        
        if (!_bs.test((h1 + h2) % M)) return false;
        if (!_bs.test((h2 + h3) % M)) return false;
        
        return true;
    }
    
    double getFillRatio() const { 
        return static_cast<double>(_bs.count()) / M; 
    }
};

// 测试不同参数配置下的布隆过滤器
template<size_t N, size_t X>
void testBloomFilter(const std::vector<std::string>& insertData, 
                     const std::vector<std::string>& testData) {
    
    BloomFilter<N, X> bf;
    
    // 计时开始
    auto start = std::chrono::high_resolution_clock::now();
    
    // 插入数据
    for (const auto& key : insertData) {
        bf.Set(key);
    }
    
    auto mid = std::chrono::high_resolution_clock::now();
    
    // 查找数据并统计误判
    int falsePositive = 0;
    
    for (const auto& key : testData) {
        if (bf.Find(key)) {
            falsePositive++;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    
    // 计算时间
    auto insertTime = std::chrono::duration_cast<std::chrono::microseconds>(mid - start);
    auto findTime = std::chrono::duration_cast<std::chrono::microseconds>(end - mid);
    
    // 输出结果
    std::cout << "参数配置: N=" << N << ", X=" << X << std::endl;
    std::cout << "  理论误判率: " << std::fixed << std::setprecision(4) 
              << bf.getFalseProbability() * 100 << "%" << std::endl;
    std::cout << "  实际误判率: " << std::fixed << std::setprecision(4) 
              << (double)falsePositive / testData.size() * 100 << "%" 
              << " (" << falsePositive << "/" << testData.size() << ")" << std::endl;
    std::cout << "  插入时间: " << insertTime.count() / 1000.0 << " ms" << std::endl;
    std::cout << "  查找时间: " << findTime.count() / 1000.0 << " ms" << std::endl;
    std::cout << std::string(50, '-') << std::endl;
}

int main() {
    std::cout << "========== 布隆过滤器复杂测试 ==========" << std::endl;
    
    // 测试参数
    const int INSERT_COUNT = 10000;     // 插入数据量
    const int TEST_COUNT = 10000;        // 测试数据量
    const int STRING_LENGTH = 10;         // 字符串长度
    
    // 生成测试数据
    std::cout << "生成测试数据..." << std::endl;
    std::vector<std::string> insertData;
    std::vector<std::string> testData;
    
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // 生成插入数据
    for (int i = 0; i < INSERT_COUNT; ++i) {
        insertData.push_back(randomString(STRING_LENGTH));
    }
    
    // 生成测试数据（一部分已存在，一部分不存在）
    std::uniform_int_distribution<> dis(0, 1);
    for (int i = 0; i < TEST_COUNT; ++i) {
        if (i < TEST_COUNT / 2 && !insertData.empty()) {  // 前50%用已存在的数据
            std::uniform_int_distribution<> pick(0, INSERT_COUNT - 1);
            testData.push_back(insertData[pick(gen)]);
        } else {  // 后50%生成随机字符串（应该不存在）
            testData.push_back(randomString(STRING_LENGTH));
        }
    }
    
    std::cout << "数据生成完成！" << std::endl;
    std::cout << "插入数据量: " << INSERT_COUNT << std::endl;
    std::cout << "测试数据量: " << TEST_COUNT << std::endl;
    std::cout << std::string(50, '=') << std::endl;
    
    // 测试1: 不同X值对误判率的影响
    std::cout << "\n【测试1: 不同X值对误判率的影响】" << std::endl;
    testBloomFilter<10000, 2>(insertData, testData);   // 每个元素2位，空间小
    testBloomFilter<10000, 5>(insertData, testData);   // 每个元素5位，空间中等
    testBloomFilter<10000, 10>(insertData, testData);  // 每个元素10位，空间大
    testBloomFilter<10000, 20>(insertData, testData);  // 每个元素20位，空间很大
    
    // 测试2: 不同数据量下的表现
    std::cout << "\n【测试2: 不同数据量下的表现】" << std::endl;
    
    // 重新生成不同规模的数据
    std::vector<int> sizes = {100, 1000, 5000, 10000};
    for (int size : sizes) {
        std::vector<std::string> smallInsert;
        
        for (int i = 0; i < size; ++i) {
            smallInsert.push_back(randomString(STRING_LENGTH));
        }
        
        std::cout << "数据规模: " << size << std::endl;
        BloomFilter<10000, 5> bf;  // 固定空间大小
        
        auto start = std::chrono::high_resolution_clock::now();
        for (const auto& key : smallInsert) {
            bf.Set(key);
        }
        auto end = std::chrono::high_resolution_clock::now();
        
        auto time = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::cout << "  插入" << size << "个元素耗时: " 
                  << time.count() / 1000.0 << " ms" << std::endl;
    }
    
    // 测试3: 哈希函数组合的影响
    std::cout << "\n【测试3: 不同哈希函数组合的影响】" << std::endl;
    
    // 测试标准版本
    std::cout << "标准版本(3个哈希函数):" << std::endl;
    BloomFilterWithCount bfStandard;
    
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& key : insertData) {
        bfStandard.Set(key);
    }
    auto mid = std::chrono::high_resolution_clock::now();
    
    int falsePositiveStd = 0;
    for (const auto& key : testData) {
        if (bfStandard.Find(key)) {
            falsePositiveStd++;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    
    auto insertTimeStd = std::chrono::duration_cast<std::chrono::microseconds>(mid - start);
    auto findTimeStd = std::chrono::duration_cast<std::chrono::microseconds>(end - mid);
    
    std::cout << "  实际误判率: " << std::fixed << std::setprecision(4) 
              << (double)falsePositiveStd / testData.size() * 100 << "%" 
              << " (" << falsePositiveStd << "/" << testData.size() << ")" << std::endl;
    std::cout << "  插入时间: " << insertTimeStd.count() / 1000.0 << " ms" << std::endl;
    std::cout << "  查找时间: " << findTimeStd.count() / 1000.0 << " ms" << std::endl;
    std::cout << "  填充率: " << bfStandard.getFillRatio() * 100 << "%" << std::endl;
    
    // 测试增强版本
    std::cout << "\n增强版本(5个哈希函数):" << std::endl;
    BloomFilterCustom bfCustom;
    
    start = std::chrono::high_resolution_clock::now();
    for (const auto& key : insertData) {
        bfCustom.Set(key);
    }
    mid = std::chrono::high_resolution_clock::now();
    
    int falsePositiveCustom = 0;
    for (const auto& key : testData) {
        if (bfCustom.Find(key)) {
            falsePositiveCustom++;
        }
    }
    end = std::chrono::high_resolution_clock::now();
    
    auto insertTimeCustom = std::chrono::duration_cast<std::chrono::microseconds>(mid - start);
    auto findTimeCustom = std::chrono::duration_cast<std::chrono::microseconds>(end - mid);
    
    std::cout << "  实际误判率: " << std::fixed << std::setprecision(4) 
              << (double)falsePositiveCustom / testData.size() * 100 << "%" 
              << " (" << falsePositiveCustom << "/" << testData.size() << ")" << std::endl;
    std::cout << "  插入时间: " << insertTimeCustom.count() / 1000.0 << " ms" << std::endl;
    std::cout << "  查找时间: " << findTimeCustom.count() / 1000.0 << " ms" << std::endl;
    std::cout << "  填充率: " << bfCustom.getFillRatio() * 100 << "%" << std::endl;
    
    // 测试4: 极端情况测试
    std::cout << "\n【测试4: 极端情况测试】" << std::endl;
    
    // 测试1: 空过滤器
    BloomFilter<1000, 5> emptyBf;
    std::cout << "空过滤器查找: " 
              << (emptyBf.Find("anything") ? "存在" : "不存在") << std::endl;
    
    // 测试2: 重复插入
    BloomFilter<100, 5> dupBf;
    dupBf.Set("same");
    dupBf.Set("same");  // 重复插入
    dupBf.Set("same");
    std::cout << "重复插入后查找: " 
              << (dupBf.Find("same") ? "存在" : "不存在") << std::endl;
    
    // 测试3: 长字符串
    std::string longStr(1000, 'a');
    BloomFilter<100, 5> longBf;
    longBf.Set(longStr);
    std::cout << "长字符串查找: " 
              << (longBf.Find(longStr) ? "存在" : "不存在") << std::endl;
    
    // 测试4: 空字符串
    BloomFilter<100, 5> emptyStrBf;
    emptyStrBf.Set("");
    std::cout << "空字符串查找: " 
              << (emptyStrBf.Find("") ? "存在" : "不存在") << std::endl;
    
    std::cout << "\n========== 测试完成 ==========" << std::endl;
    
    return 0;
}