//
//  main.cpp
//  Threads
//
//  Created by Екатерина Вишневская on 19.02.16.
//  Copyright © 2016 Екатерина Вишневская. All rights reserved.
//

#include <iostream>
#include <thread>
#include <mutex>
#include <atomic>

std::atomic<int> i(0);
std::atomic_flag flag;
std::mutex mut;

class MyMutex
{
    
};

void f(int id){
    flag.clear();
    flag.test_and_set();
    i.store(10);

    if (i.load()==10)
    {
        std::cout<<"Bingo!";
    }
}

void h(){
    
    for (int j=0; j<1000000; j++)
    {
        mut.lock();
        i++;
        mut.unlock();
    }
}

void g(){
    
    for (int j=0; j<1000000; j++)
    {
        std::lock_guard<std::mutex> guard(mut);// RAII - unlock в деструкторе;
        i++;
    }
}

void m(){
    
    for (int j=0; j<1000000; j++)
    {
        
        std::unique_lock<std::mutex> guard(mut, std::adopt_lock_t());// RAII - unlock в деструкторе;
        mut.lock();
        i++;
        //mut.unlock();
    }
}

void n(){
    
    for (int j=0; j<1000000; j++)
    {
        
        std::unique_lock<std::mutex> guard(mut, std::defer_lock_t());// RAII - unlock в деструкторе;
        mut.lock();
        i++;
        //mut.unlock();
    }
}

int main() {
    // insert code here...
    std::thread obj1(n, 0);
    std::thread obj2(n, 1);
    obj1.join();
    obj2.join();
    std::cout << i;
    return 0;
}
