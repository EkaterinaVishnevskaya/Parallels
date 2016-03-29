//
//  main.cpp
//  Stack
//
//  Created by Екатерина Вишневская on 04.03.16.
//  Copyright © 2016 Екатерина Вишневская. All rights reserved.
//

#include <iostream>
#include <stack>
#include <mutex>
#include <thread>
#include <condition_variable>

using namespace std;

template<class T>
class MTS
{
public:
    void push(T&& obj)
    {
        unique_lock<mutex> lock(mut);
        internalStack.push(obj);
        lock.unlock();
        condVar.notify_one();
    };
    T wait_pop()
    {
        lock_guard<mutex> guard(mut);

        condVar.wait(guard, [this]() {return (internalStack.siz()>0);});
                //assert(guard.locked());
        T result(internalStack.top());
        internalStack.pop();
        return result;
    };
    pair<T, bool> try_pop()
    {
        lock_guard<mutex> guard(mut);
        if (!internalStack.empty())
        {
            T result(internalStack.top());
            internalStack.pop();
            return make_pair (result, true);
        } else
        {
            return pair <T, bool> (T(), false);
        }
    
    };
private:
    stack<T> internalStack;
    mutex mut;
    condition_variable condVar;
};

void f(int a){}

int main() {
    MTS<int> st;
    st.push(10);
    st.wait_pop();
    cout << "Hello, World!\n";
    return 0;
}
