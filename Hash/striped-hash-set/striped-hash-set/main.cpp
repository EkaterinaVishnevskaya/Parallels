//
//  main.cpp
//  striped-hash-set
//
//  Created by Екатерина Вишневская on 10.04.16.
//  Copyright © 2016 Екатерина Вишневская. All rights reserved.
//

#include <iostream>
#include <vector>
#include <list>
#include <forward_list>
#include <shared_mutex>
#include <array>

template <typename ElementType, class HashFunction = std::hash<ElementType>>
class HashTable{
public:
    void add(const ElementType& t) {
        int hash = HashFunction(t);
        size_t lockNumber = hash%locks.size();
        std::lock_guard<std::shared_mutex> sharedLocker(mut);
        size_t buckNum = hash%data.size();
        for (auto iter:data[buckNum])
        {
            if (Comparator()(iter, t)){
                return;
            }
        }
        data[buckNum].push_front(t);
        size++;
        //ResizeIfNeeded();
    }
    //void add(const ElementType& t, const HashFunction& hasher) {
    //    int hash = hasher(t);
    //}
private:
    std::vector<std::shared_timed_mutex> locks;
    std::atomic<int> size;
    const double policy;
    std::vector<std::forward_list<ElementType>> data;
};

struct Str {
    int a;
    int b;
};

namespace std {
    template<>
    struct hash<Str>
    {
        size_t operator ()(const Str& key) const
        {
            return key.a+(key.b << 16);
        }
    };
}

struct SHash
{
    size_t operator ()(const Str& key) const
    {
        return key.a+(key.b << 16);
    }
};

int main(){
    
    HashTable<Str> hashtab;
    hashtab.add(Str());
    return 0;
}
