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
#include <memory>
#include <algorithm>
#include <iterator>
#include <functional>
#include <condition_variable>
#include <deque>
#include "../ds/ConcurrentQueue.h"
namespace conet {

template<typename T>
class ThreadPool {
public:
    typedef T Task;

    ThreadPool(int size);
    ~ThreadPool() { if (running_) stop(); } // 防止重复调用 stop
    void start();
    void stop();
    void add_task(Task t);

private:
    void worker_thread();
    Task take();
private:
    std::atomic_bool running_;
    size_t num_thread_;
    std::mutex mutex_;
    std::condition_variable cond_;
    std::vector<std::thread> workers_;
    std::deque<Task> tasks_;
};

template<typename T>
ThreadPool<T>::ThreadPool(int size) : running_(false) {
    if (size < 0) num_thread_ = std::thread::hardware_concurrency();
    else num_thread_ = size;
}

template<typename T>
void ThreadPool<T>::start() {
    running_ = true;
    std::generate_n(std::back_inserter(workers_), num_thread_, [&]{
        return std::move(std::thread(std::bind(&ThreadPool::worker_thread, this)));
    });
} 

template<typename T>
void ThreadPool<T>::stop() {
    {
        std::lock_guard<std::mutex> lck(mutex_);
        running_ = false;
        cond_.notify_all();
    }
    std::for_each(workers_.begin(), workers_.end(), [&](std::thread & t){
        t.join();
    });
}

template<typename T>
void ThreadPool<T>::add_task(Task t) {
    std::lock_guard<std::mutex> lck(mutex_);
    tasks_.push_back(t);
    cond_.notify_one();
}

template<typename T>
T ThreadPool<T>::take() {
    // 等待队列中有元素
    std::unique_lock<std::mutex> lck(mutex_);
    while (tasks_.empty() && running_) {
        cond_.wait(lck);
    }
    Task t;
    if (!tasks_.empty()) { // 需要判断是不是 running_为false而退出循环
        t = tasks_.front();
        tasks_.pop_front();
    }
    return t;
}

template<typename T>
void ThreadPool<T>::worker_thread() {
    Task t;
    while (running_) {
        t = take(); // 让线程阻塞在条件变量上
        if (t) {
            t();
        }
    }
}

}