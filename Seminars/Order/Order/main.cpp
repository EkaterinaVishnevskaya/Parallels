//
//  main.cpp
//  Order
//
//  Created by Екатерина Вишневская on 22.04.16.
//  Copyright © 2016 Екатерина Вишневская. All rights reserved.
//

#include <iostream>
#include <atomic>
#include <thread>

int x=0;
int y=0;
std::atomic_int sum;

void f1()
{
    x = 1;
    std::atomic_signal_fence(std::memory_order_seq_cst);
    int local = y;
    sum.fetch_add(local);
}

void f2()
{
    y = 1;
    std::atomic_signal_fence(std::memory_order_seq_cst);
    int local = x;
    sum.fetch_add(local);
}

int main()
{
    for (int i=0; i<100000; i++)
    {
        x=0;
        y=0;
        sum.store(0);
        std::thread th1(f1);
        std::thread th2(f2);
        th1.join();
        th2.join();
        if (sum.load()==0)
        {
            std::cout << "success" << std::endl;
        }
    }
}
