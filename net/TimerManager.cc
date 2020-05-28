/*
 * @Description: 
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-27 14:28:54
 */ 
#include "TimerManager.h"

#include <sys/timerfd.h>
#include <functional>
#include <unistd.h>
#include "EventLoop.h"
#include "Channel.h"
#include "Timer.h"
#include <../utils/TimeStamp.h>
#include "../log/Logger.h"

using namespace conet;

namespace {
    int create_timerfd() {
        int fd = ::timerfd_create(CLOCK_MONOTONIC, TFD_CLOEXEC | TFD_NONBLOCK);
        if (fd < 0) {
            LOG_FATAL("TimerManger::create_timerfd error.");
        }
        return fd;
    }

    void read_timerfd(int timerfd) {
        // 当定时器超时，read读事件发生即可读，返回超时次数（从上次调用timerfd_settime()启动开始或上次read成功读取开始），
        // 它是一个8字节的unit64_t类型整型
        uint64_t how;
        ssize_t n = ::read(timerfd, &how, sizeof(how));
        if (n != sizeof(how)) {
            LOG_ERROR("read_timerfd()");
        }
    }

    struct itimerspec to_itimerspec(TimeStamp t) {
        int64_t ms = t.micro_sec();
        struct itimerspec its;
        struct timespec ts;
        ts.tv_sec = static_cast<time_t>(ms / TimeStamp::kMicroPerSec);
        ts.tv_nsec = static_cast<time_t>((ms % TimeStamp::kMicroPerSec) * 1000);
        its.it_value = ts;
        return its;
    }

    void reset_timerfd(int timer_fd, TimeStamp when) {
        TimeStamp diff = when - TimeStamp::now();
        struct itimerspec new_value = to_itimerspec(diff);
        int status = ::timerfd_settime(timer_fd, 0, &new_value, nullptr);
        if (status) {
            LOG_ERROR("timerfd_settime()");
        }
    }
}

struct TimerManager::Entry {
public:
    Entry(const Timer::TimerCallback &cb, TimeStamp when, double interval)
        : timer_(std::make_unique<Timer>(cb, when, interval)) {}
    int id() const { return timer_->sequence(); }
    void run() { timer_->run(); }
    TimeStamp when() const { return timer_->expiration(); }
    bool operator<(const Entry& other) {
        return timer_->expiration() <= other.timer_->expiration();
    }
private:
    std::unique_ptr<Timer> timer_;
}

TimerManager::TimerManager(EventLoop * loop)
    : owner_loop_(loop),
      timer_fd_(::create_timerfd()),
      timer_channel_(owner_loop_, timer_fd_)
      timers_()
{
    timer_channel_.set_read_cb(std::bind(&TimerManager::handle_read, this));
    timer_channel_.enabel_read();
}

void TimerManager::handle_read() {
    // 需要先把 fd 中的内容读出来，防止一直处于可读状态
    ::read_timerfd(timer_fd_);
    tick(); // 处理到期的定时器
}

bool TimerManager::insert() {

}

void TimerManager::tick() {
    // TODO: implement tick function
    TimeStamp now = TimeStamp::now(); // current time
    std::vector<std::unique_ptr<Timer> > expired_timers; // 已经到期的定时器列表
    // 获取已经到期的定时器
    while (!timers_.empty()) {
        const Timer & timer = timers_.top();
        if (timer.expiration() < now) {
            // 已经到期
            // TODO: pop不返回，怎么把它加入到 expired_timers; move(const reference) 是否可行
            expired_timers.push_back(std::move(timer)); // ???
            timers_.pop();
        }
    }
    auto it = timers_.begin();
    for (; it != timers_.end(); it++) {
        if (*it.when() > now) break;
        // 执行定时任务
        *it.run();
        expired_timers.push_back(std::move(*it));
    }
    // 从定时器列表中将已到期的定时器删除
    timers_.erase(timers_.begin(), it);
    it = expired_timers.begin();
    for (; it !+ expired_timers.end(); it ++) {

    }
}