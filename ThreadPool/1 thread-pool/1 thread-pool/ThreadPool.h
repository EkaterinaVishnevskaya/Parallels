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
#include<queue>

template<class T>
class ThreadQueue
{
public:
    void push(T&& obj)
    {
        std::unique_lock<std::mutex> lock(mut);
        internalQueue.push(obj);
        lock.unlock();
        condVar.notify_one();
    };
    T wait_pop()
    {
        std::unique_lock<std::mutex> guard(mut);
        
        condVar.wait(guard, [this]() {return (internalQueue.size()>0);});
        //assert(guard.locked());
        T result(internalQueue.front());
        internalQueue.pop();
        return result;
    };
    std::pair<T, bool> try_pop()
    {
        std::lock_guard<std::mutex> guard(mut);
        if (!internalQueue.empty())
        {
            T result(internalQueue.top());
            internalQueue.pop();
            return make_pair (result, true);
        } else
        {
            return std::pair <T, bool> (T(), false);
        }
        
    };
private:
    std::queue<T> internalQueue;
    std::mutex mut;
    std::condition_variable condVar;
};

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
    static const size_t DEFAULT_THREADS_COUNT = 4;
};

template<typename T>
ThreadPool<T>::ThreadPool(size_t threads_count){
    if (threads_count == 0) threads_count = DEFAULT_THREADS_COUNT;
    
    auto workerFunction = [this]() -> void {
        Task currentTask(taskQueue.wait_pop());
        while (currentTask.job != nullptr) {
            currentTask.promise_ptr->set_value(currentTask.job());
            
            currentTask = taskQueue.wait_pop();
        };
    };
    
    for (size_t i = 0; i < threads_count; i++)
        workers.push_back(std::thread(workerFunction));
}

template<typename T>
ThreadPool<T>::~ThreadPool(){
    for (size_t i = 0; i < workers.size(); i++)
        taskQueue.push(Task(nullptr, nullptr));
    for (auto& thread : workers)
        if (thread.joinable()) thread.join();
}

template<typename T>
std::shared_ptr<std::future<T>> ThreadPool<T>::submit(std::function<T()> functionToPerform){
    auto pointerToPromise = std::make_shared<std::promise<T>>(std::promise<T>());
    taskQueue.push(Task(functionToPerform, pointerToPromise));
    return std::make_shared<std::future<T>>(pointerToPromise->get_future());
}
