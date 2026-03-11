#pragma once
#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <algorithm>

namespace HashTable_Model
{
    using namespace std;

    template<class K,class V>
    struct HashNode
    {
        HashNode(const pair<K,V>& kv)
            :_kv(kv)
            ,_next(nullptr)
        {}

        pair<K,V> _kv;
        HashNode<K,V>* _next;
    };

    // key提取
    template<class K>
    struct KeyOfT
    {
        const K& operator()(const K& key) const
        {
            return key;
        }
    };

    // 默认hash
    struct Hash
    {
        size_t operator()(int n) const
        {
            return n < 0 ? -n : n;
        }

        size_t operator()(const string& str) const
        {
            size_t hash = 0;
            for (auto ch : str)
            {
                hash = hash * 131 + ch;
            }
            return hash;
        }
    };

    template<class K,class V,class KeyOfT=KeyOfT<K>,class HashT=Hash>
    class HashTable
    {
        using Node = HashNode<K,V>;

    public:

        HashTable()
            :_n(0)
        {
            _tables.resize(53,nullptr);
        }

        bool Insert(const pair<K,V>& kv)
        {
            if(Find(kv.first))
                return false;

            CheckCapacity();

            size_t hash = _hash(_kft(kv.first));
            size_t index = hash % _tables.size();

            Node* newnode = new Node(kv);
            newnode->_next = _tables[index];
            _tables[index] = newnode;

            ++_n;

            return true;
        }

        Node* Find(const K& key)
        {
            if(_tables.empty()) return nullptr;

            size_t hash = _hash(_kft(key));
            size_t index = hash % _tables.size();

            Node* cur = _tables[index];

            while(cur)
            {
                if(cur->_kv.first == key)
                    return cur;

                cur = cur->_next;
            }

            return nullptr;
        }

        bool Erase(const K& key)
        {
            size_t hash = _hash(_kft(key));
            size_t index = hash % _tables.size();

            Node* cur = _tables[index];
            Node* prev = nullptr;

            while(cur)
            {
                if(cur->_kv.first == key)
                {
                    if(prev)
                        prev->_next = cur->_next;
                    else
                        _tables[index] = cur->_next;

                    delete cur;
                    --_n;
                    return true;
                }

                prev = cur;
                cur = cur->_next;
            }

            return false;
        }

        V& operator[](const K& key)
        {
            Node* node = Find(key);

            if(node)
                return node->_kv.second;

            Insert(make_pair(key,V()));
            node = Find(key);

            return node->_kv.second;
        }

    private:

        void CheckCapacity()
        {
            if(_n*10/_tables.size() >= 7)
            {
                size_t newSize = _GetNextPrime(_tables.size());

                vector<Node*> newTables;
                newTables.resize(newSize,nullptr);

                for(size_t i=0;i<_tables.size();i++)
                {
                    Node* cur = _tables[i];

                    while(cur)
                    {
                        Node* next = cur->_next;

                        size_t hash = _hash(_kft(cur->_kv.first));
                        size_t index = hash % newSize;

                        cur->_next = newTables[index];
                        newTables[index] = cur;

                        cur = next;
                    }
                }

                _tables.swap(newTables);
            }
        }

        size_t _GetNextPrime(size_t n)
        {
            static const int primeList[] =
            {
                53,97,193,389,769,
                1543,3079,6151,12289,24593,
                49157,98317,196613,393241,786433,
                1572869,3145739,6291469,12582917,25165843,
                50331653,100663319,201326611,402653189,805306457,
                1610612741
            };

            for(auto prime : primeList)
            {
                if(prime > n)
                    return prime;
            }

            return primeList[sizeof(primeList)/sizeof(primeList[0])-1];
        }

    private:

        vector<Node*> _tables;

        size_t _n;

        KeyOfT _kft;

        HashT _hash;
    };
}