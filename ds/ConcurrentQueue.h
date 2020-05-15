/*
 * @Description: 细粒度的线程安全队列
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-14 16:54:19
 */
#pragma once
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
namespace conet {

template <typename T>
class ConcurrentQueue {

public:
    ConcurrentQueue() : head_(new node), tail_(head_.get()) {}
    ~ConcurrentQueue() { cond_.notify_all(); } // 销毁的时候唤醒所有阻塞在队列上的线程
    ConcurrentQueue(const ConcurrentQueue&) = delete;
    ConcurrentQueue& operator=(const ConcurrentQueue&) = delete;
public:
    void put(T value);
    void take(T & value);
    bool try_take(T & value);
    bool empty();
private:
    struct node
    {
        std::shared_ptr<T> data;
        std::unique_ptr<node> next;
    };
    node* get_tail() {
        std::lock_guard<std::mutex> lk(tail_mutex_);
        return tail_;
    }
    std::unique_ptr<node> pop_head() {
        // std::lock_guard<std::mutex> lk(head_mutex_);
        std::unique_ptr<node> old_head = std::move(head_);
        head_ = std::move(old_head->next);
        return old_head; 
    }

private:
    std::mutex head_mutex_; // 保护头结点和尾节点
    std::mutex tail_mutex_;
    std::condition_variable cond_;

    std::unique_ptr<node> head_; // 两个哨兵节点
    node * tail_;
};

template<typename T>
void ConcurrentQueue<T>::put(T value) {
    auto new_data { std::make_shared<T>(std::move(value)) };
    auto new_tail { std::make_unique<node>() };
    {
        std::lock_guard<std::mutex> lk(tail_mutex_);
        tail_->data = new_data;
        tail_->next = std::move(new_tail);
        tail_ = tail_->next.get();
    };
    cond_.notify_one();
}

template<typename T>
void ConcurrentQueue<T>::take(T & value) {
    // 等待有数据
    std::unique_lock<std::mutex> lck(head_mutex_);
    cond_.wait(lck, [&]{return head_.get() != get_tail();});
    // 获取栈顶元素
    value = std::move(*head_->data);
    // 弹出栈顶
    head_ = std::move(head_->next);
}

template<typename T>
bool ConcurrentQueue<T>::try_take(T & value) {
    std::lock_guard<std::mutex> lck(head_mutex_);
    if (head_.get() == get_tail()) return false;
    value = std::move(*head_->data);
    head_ = std::move(head_->next);
    return true;
}

template<typename T>
bool ConcurrentQueue<T>::empty() {
    std::lock_guard<std::mutex> lck(head_mutex_);
    return head_.get() == get_tail();
}

}