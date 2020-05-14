/*
 * @Description: 使用C++11的Tread库封装一个线程池
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-13 15:29:45
 */
#pragma once
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <functional>
#include <memory>
#include <algorithm>
#include <iterator>
#include <condition_variable>
#include "../ds/ConcurrentQueue.h"
namespace conet {

// class Task {
// public:
//     Task(std::function<void()> cb) : cb_(cb) {}
// private:
//     std::function<void()> cb_;
// };

// class TaskExecutor {
// public:
//     TaskExecutor();
//     void start() {
//         thread_ = std::make_unique<std::thread>([&]{ this->execute(); }
//     }
// private:
//     void execute() {

//     }
// private:
//     std::unique_ptr<std::thread> thread_;
//     ConcurrentQueue<Task> tasks_;
// };

class ThreadPool {
public:
    typedef std::function<void()> Task;

    ThreadPool(int size);
    ~ThreadPool();
    void start();
    void stop();
    void add_task(Task t);

private:
    void worker_thread();
private:
    std::atomic_bool running_;
    size_t num_thread_;
    std::vector<std::thread> workers_;
    ConcurrentQueue<std::function<void()>> tasks_;
};

ThreadPool::ThreadPool(int size) : running_(false) {
    if (size < 0) num_thread_ = std::thread::hardware_concurrency();
    else num_thread_ = size;
}

void ThreadPool::start() {
    std::generate_n(std::back_inserter(workers_), num_thread_, [&]{
        return std::move(std::thread(std::bind(&ThreadPool::worker_thread, this)));
    });
} 

void ThreadPool::stop() {
    running_ = false;
    
    std::for_each(workers_.begin(), workers_.end(), [&](std::thread & t){
        t.join();
    });
}

void add_task(Task t) {
    tasks_.put(t);
}

void ThreadPool::worker_thread() {
    Task t;
    while (running_) {

    }
}