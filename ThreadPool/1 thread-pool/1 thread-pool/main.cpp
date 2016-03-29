//
//  main.cpp
//  1 thread-pool
//
//  Created by Екатерина Вишневская on 15.03.16.
//  Copyright © 2016 Екатерина Вишневская. All rights reserved.
//

#include "ThreadPool.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <vector>
#include <future>
#include <math.h>

bool isPrime(int n){
    for (int i = 2; i <= sqrt(n); i++)
        if (n%i == 0)
            return false;
    return true;
}

int main(){
    std::srand(std::time(0));
    
    std::vector<int> values;
    std::vector<std::shared_ptr<std::future<bool>>> results;
    ThreadPool<bool> pool;
    
    for (int i = 0; i < 10; i++){
        values.push_back(std::rand());
        results.push_back(pool.submit(std::bind(isPrime, values[i])));
    }
    
    for (int i = 0; i < 10; i++){
        std::cout << "number " << values[i] << (results[i]->get() ? " is prime" : " is not prime") << std::endl;
    }
    
    return 0;
}
