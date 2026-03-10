#pragma once
#include <iostream>
#include <vector>

namespace HashTable_Model
{
    template<K,V>
    struct hashNode
    {
        pair<K,V> _kv;
        hashNode<K,V>* _next;
    };
    template<K,V,KeyOfT,Hash>
    class HashTable
    {
        using Node = hashNode<K,V>;
    public:
        HashTable()
        {
            __stl_next_prime(0);
        }
        inline unsigned long __stl_next_prime(unsigned long n)
        {
            // Note: assumes long is at least 32 bits.
            static const int __stl_num_primes = 28;
            static const unsigned long __stl_prime_list[__stl_num_primes] =
                {53, 97, 193, 389, 769,
                1543, 3079, 6151, 12289, 24593,
                49157, 98317, 196613, 393241, 786433,
                1572869, 3145739, 6291469, 12582917, 25165843,
                50331653, 100663319, 201326611, 402653189, 805306457,
                1610612741, 3221225473, 4294967291
                };
            const unsigned long* first = __stl_prime_list;
            const unsigned long* last = __stl_prime_list +
            __stl_num_primes;
            const unsigned long* pos = lower_bound(first, last, n);
            return pos == last ? *(last - 1) : *pos;
        }
        bool insert(pair<K,V> kv)
        {
            if(!Find(kv)) return false;
            if(_n*10/_hashtables.size()>=7)//过载，进行扩容
            {

            }
        }
        bool opeator[](K key, V val)
        {

        }
        bool Find(pair<K,V> kv)
        {

        }
        bool erase()
        {

        }
    private:
        std::vector<Node> _hashtables;
        size_t _n = 0;
    };
};