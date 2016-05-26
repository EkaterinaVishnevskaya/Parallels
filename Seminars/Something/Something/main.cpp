//
//  main.cpp
//  Something
//
//  Created by Екатерина Вишневская on 29.04.16.
//  Copyright © 2016 Екатерина Вишневская. All rights reserved.
//

#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>
#include <vector>

class tas_spinlock
{
private:
    std::atomic<bool> val;
    char padding [64-sizeof(val)];
public:
    void lock();
    void unlock();
    
};

void tas_spinlock::lock()
{
    while(val.exchange(true, std::memory_order_acquire)){}
}

void tas_spinlock::unlock() {
    val.store(false, std::memory_order_release);
}

class tatas_spinlock
{
private:
    std::atomic<bool> val;
public:
    void lock();
    void unlock();
    
};

void tatas_spinlock::lock() {
    
    while (true)
    {
        while (val.load(std::memory_order_relaxed)){}
        if (!val.exchange(true, std::memory_order_acquire))
        {
            return;
        }
    }
}

void tatas_spinlock::unlock() {
    val.store(false, std::memory_order_release);
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "Hello, World!\n";
    return 0;
}
