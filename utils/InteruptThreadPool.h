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
template<typename T>
class ThreadPool {
public:
    typedef T Task;

    ThreadPool(int size);
    ~ThreadPool() { stop(); }
    void start();
    void stop();
    void add_task(Task t);

private:
    void worker_thread();
private:
    std::atomic_bool running_;
    size_t num_thread_;
    // 注意这里的 workers_和tasks_的定义顺序不能更换
    // 队列会先调用析构函数，在析构函数中会唤醒所有阻塞的线程
    // 这样 workers_ 内的线程才会自行退出，否则会一直阻塞
    // TODO: 还有问题，即使唤醒了所有线程，还会有线程阻塞在队列上
    // 如果在线程阻塞的时候队列销毁了，会引起 Core dump 程序直接崩溃
    // 还需要给线程增加一个中断的标记，使得线程可以自我唤醒
    std::vector<std::thread> workers_;
    ConcurrentQueue<Task> tasks_;
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
    running_ = false;

    std::for_each(workers_.begin(), workers_.end(), [&](std::thread & t){
        t.join();
    });
}

template<typename T>
void ThreadPool<T>::add_task(Task t) {
    tasks_.put(t);
}

template<typename T>
void ThreadPool<T>::worker_thread() {
    Task t;
    while (running_) {
        tasks_.take(t); // 让线程阻塞在条件变量上
        t();
        // 下面这种写法是 busy-loop 的
        // https://www.zhihu.com/question/30432095
        // if (tasks_.try_take(t)) {
        //     t();
        // } else {
        //     std::this_thread::yield();
        // }
    }
}

}