#pragma once
#include <vector>
#include <list>
#include <utility>
#include <functional>

// 简单哈希表：链地址法
template <class K, class V>
class HashMap {
    std::vector<std::list<std::pair<K, V>>> buckets;
    std::size_t hash(const K& k) const { return std::hash<K>{}(k) % buckets.size(); }

public:
    HashMap(std::size_t n = 16) : buckets(n) {}

    void put(const K& k, const V& v) {
        auto& b = buckets[hash(k)];
        for (auto& kv : b)
            if (kv.first == k) { kv.second = v; return; }  // 已存在则更新
        b.emplace_back(k, v);
    }

    V* get(const K& k) {
        for (auto& kv : buckets[hash(k)])
            if (kv.first == k) return &kv.second;
        return nullptr;  // 找不到返回空指针
    }

    bool erase(const K& k) {
        auto& b = buckets[hash(k)];
        for (auto it = b.begin(); it != b.end(); ++it)
            if (it->first == k) { b.erase(it); return true; }
        return false;
    }
};
