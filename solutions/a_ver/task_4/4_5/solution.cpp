#include <iostream>
#include <thread>
#include <future>
#include <vector>

#include "solution.h"

typedef int& (reduce_function_t)(int &, int&);

task4_5::solution::solution( const data_type& data ): _data(std::move(data))
{
}

int reduce(
    reduce_function_t function,
    const std::vector<int>& list)
{
    int acc = list.front();
    for(auto it=list.begin()+1; it != list.end(); it++){
        int v = (*it);
        acc = function(acc, v);
    }
    return acc;
}

void reduce_thread(
    reduce_function_t function,
    const std::vector<int>& list,
    std::promise<int>&& p)
{
    int r = reduce(function, list);
    p.set_value(r);
}

void threads_join(std::vector<std::thread> &threads){
    for(auto &t : threads){
        t.join();
    }
}

int reduce_multi_thread(
    reduce_function_t function,
    const task4_5::data_type& _data)
{
    if(_data.size() == 0){
        return 0;
    }
    std::vector<std::thread> threads;
    std::vector< std::future<int> > futures;
    std::vector<int> results;

    for(auto it=_data.begin(); it != _data.end(); it++){
        std::promise<int> p;
        futures.push_back(p.get_future());
        threads.push_back(
            std::thread(reduce_thread, function, std::ref(*it), std::move(p)));
    }
    threads_join(threads);
    for(auto it = futures.begin(); it != futures.end(); it++){
        it->wait();
        results.push_back(it->get());
    }
    return reduce(function, results);
}

int task4_5::solution::get_min() const
{
    auto fn = [](int& acc, int& v)->int&{ return (acc<v)? acc: v; };
    return reduce_multi_thread(fn, _data);
}

int task4_5::solution::get_max() const
{
    auto fn = [](int& acc, int& v)->int&{ return (acc>v)? acc: v; };
    return reduce_multi_thread(fn, _data);
}
