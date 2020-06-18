/*
 * @Description: 
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-22 09:36:57
 */
#pragma once
#include <functional>
#include <atomic>
#include "../utils/TimeStamp.h"

namespace conet {

typedef int64_t TimerId;
class Timer {
public:
    typedef std::function<void()> Functor;
    Timer(std::function<void()> cb, TimeStamp when, double interval);
    void run() const { cb_(); }
    ~Timer() = default;
    TimeStamp expiration() const { return expiration_; }
    bool repeat() const { return repeat_; }
    TimerId id() const { return sequence_id_; }
    bool operator<(const Timer & other) const {
        return expiration_ <= other.expiration();
    }
    void restart(TimeStamp now);
private:
    static std::atomic<int64_t> num_created_;
    TimeStamp expiration_; // 超时时间
    Functor cb_; // 回调函数
    const int64_t sequence_id_; // 编号
    const double interval_; // 间隔
    const bool repeat_; // 是否是重复定时器
};
}