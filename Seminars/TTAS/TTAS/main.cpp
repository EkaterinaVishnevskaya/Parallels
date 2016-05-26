//
//  main.cpp
//  TTAS
//
//  Created by Екатерина Вишневская on 15.04.16.
//  Copyright © 2016 Екатерина Вишневская. All rights reserved.
//

#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <vector>

class tatas_spinlock
{
private:
    std::atomic<bool> flag;
public:
    void lock();
    void unlock();
    
};

class tas_spinlock
{
private:
    std::atomic_flag flag;
public:
    void lock();
    void unlock();
    
};

void tas_spinlock::lock() {
    
    while (flag.test_and_set())
    {
    }
}

void tatas_spinlock::lock() {
    
    while (true)
    {
        while (flag.load()){}
        if (!flag.exchange(true))
        {
            return;
        }
    }
}

void tas_spinlock::unlock() {
    flag.clear();
}

void tatas_spinlock::unlock() {
    flag.store(false);
}

template<typename LockType>
void test()
{
    int value = 0;
    LockType mut;
    std::vector<std::thread> threads;
    for (int i = 0; i < 4; i++)
    {
        threads.emplace_back([&value, &mut](){
            for (int j=0; j<1000000; j++){
                std::lock_guard<LockType> guard(mut);
                value++;
            }
        });
    }
    for (auto& j:threads){
        j.join();
    }
}

template<typename LockType>
void ChronoTest(){
    auto begin = std::chrono::high_resolution_clock::now();
    test<LockType>();
    auto diff = std::chrono::high_resolution_clock::now()-begin;
    std::cout << typeid(LockType).name() << " " << diff.count() << std::endl;
}


int main(int argc, const char * argv[]) {
    // insert code here...
    ChronoTest<tas_spinlock>();
    ChronoTest<tatas_spinlock>();
    return 0;
}
