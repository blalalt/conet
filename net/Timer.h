/*
 * @Description: 
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-22 09:36:57
 */
#include <functional>
#include <atomic>
#include "../utils/TimeStamp.h"
namespace conet {

class Timer {
public:
    typedef std::function<void()> TimerCallback;
    Timer(const TimerCallback & cb, TimeStamp when, double interval)
        : expiration_(when),
          interval_(interval),
          repeat_(interval > 0.0),
          sequence_id_(num_created_++)
    void run() const { cb_(); }
    ~Timer() = default;
    TimeStamp expiration() const { return expiration_; }
    bool repeat() const { return repeat_; }
    int64_t sequence() const { return sequence_id_; }
    void restart(TimeStamp now) {
        // 每次执行完 定时器相关回调后，会调用restart，
        if (repeat_) {
            expiration_ = now + interval_; // 设置新的超时时间
        }
        else {
            expiration_ = TimeStamp::invalid();
        }
    }
private:
    static std::atomic<int64_t> num_created_;
    const TimeStamp expiration_; // 超时时间
    TimerCallback cb_; // 回调函数
    const int64_t sequence_id_; // 编号
    const double interval_; // 间隔
    const bool repeat_; // 是否是重复定时器
};

std::atomic<int64_t> Timer::num_created_;

class TimerID {
public:
    TimerID() : timer_(nullptr), seq_(0) {}
    TimerID(Timer *t, int64_t seq) : timer_(t), seq_(seq) {}

private:
    Timer * timer_;
    int64_t seq_;
};

}