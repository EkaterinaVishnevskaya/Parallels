//
//  main.cpp
//  Futures
//
//  Created by Екатерина Вишневская on 26.02.16.
//  Copyright © 2016 Екатерина Вишневская. All rights reserved.
//

#include <iostream>
#include <thread>
#include <vector>
#include <future>

void f (std::promise<int> prom)
{
    try
    {
        int result = 0;
        for (int i=0; i<2000000000; i++)
        {
            result++;
        }
        std::cout << result << std::endl;
        prom.set_value(result);
    }
    catch (...)
    {
       // prom.set_exception( std::exception_ptr::_Current_exception() );
    }
}

int f2 ()
{
    return 2;
}

void merge_sort(std::vector<int> &v, int l, int r)
{
    if ((r-l)/2<=16)
    {
        std::sort(v.at(l), v.at(r));
    }
    else 
    {
        std::future<void> fut0 = std::async(merge_sort, v, l, (r+l)/2-1);
        std::future<void> fut1 = std::async(std::launch::deferred | std::launch::async, merge_sort, v, (r+l)/2, r);
        int p=l;
        int q=(r+l)/2;
        std::vector<int> w;
        for (int i=l; i<=r; i++)
        {
            if(v[p]>v[q])
            {
                w.push_back(v[p]);
                p++;
            }
            else
            {
                w.push_back(v[q]);
                q++;
            }
        }
        for (int i=l; i<=r; i++)
        {
            v[i]=w[i-l];
        }
    }
}

int main(int argc, const char * argv[]) {
    //std::promise<int> prom;
    //std::packaged_task<int(int)> task(f2);
    
    //std::future<int> fut = prom.get_future();
    //std::future<int> fut1 = task.get_future();
    //std::thread th(f, std::move(prom));
    //std::thread th1(std::move(task), 5);
    //std::cout << fut.get() << std::endl;
    //std::cout << fut1.get() << std::endl;
    //th.join();
    //th1.join();
    
    std::vector<int> v;
    for (int i=0; i<=10; i++)
    {
        v.push_back(10-i);
    }
    
    merge_sort(v, 0, 10);
    
    for (int i=0; i<=10; i++)
    {
        std::cout << v[i];
    }
    
    return 0;
}
