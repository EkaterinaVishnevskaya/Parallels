//
//  Header.h
//  1 thread-pool
//
//  Created by Екатерина Вишневская on 16.03.16.
//  Copyright © 2016 Екатерина Вишневская. All rights reserved.
//

#ifndef Header_h
#define Header_h


#endif /* Header_h */

#pragma once

#include<memory>
#include<future>
#include<thread>
#include<vector>

template<typename T>
class ThreadPool{
public:
    ThreadPool(size_t workers_count = std::thread::hardware_concurrency());
    ~ThreadPool();
    std::shared_ptr<std::future<T>> submit(std::function<T()> functionToPerform);
private:
    std::vector<std::thread> workers;
    struct Task{
        std::function<T()> job;
        std::shared_ptr<std::promise<T>> promise_ptr;
        Task() : job(nullptr), promise_ptr(nullptr){}
        Task(std::function<T()> j, std::shared_ptr<std::promise<T>> p) : job(j), promise_ptr(p){}
    };
    ThreadQueue<Task> taskQueue;
    static const size_t DEFAULT_THREADS_COUNT = 5;
};

template<typename T>
ThreadPool<T>::ThreadPool(size_t threads_count = std::thread::hardware_concurrency()): taskQueue(100){
    if (threads_count == 0) threads_count = DEFAULT_THREADS_COUNT;
    
    auto workerFunction = [this]() -> void {
        Task currentTask(taskQueue.pop());
        while (currentTask.job != nullptr) {
            currentTask.promise_ptr->set_value(currentTask.job());
            
            currentTask = taskQueue.pop();
        };
    };
    
    for (size_t i = 0; i < threads_count; i++)
        workers.push_back(std::thread(workerFunction));
}
