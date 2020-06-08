/*
 * @Description: 
 * 1.采用timerfd_*将超时任务转换成文件描述符进行监听
   2.当时间一到，timerfd变为可读，相应的Channel调用回调函数（TimerQueue::handleRead）
   3.回调函数中将所有在TimerQueue中的超时任务找出，一次调用其回调函数
   4.对于周期性定时任务，再添加回TimerQueue中
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-22 08:46:16
 */ 
#include <set>
#include <Timer.h>
#include <memory>
#include <list>
#include <vector>
#include <queue>
#include "../utils/TimeStamp.h"
namespace conet {

class TimerManager {
//TODO: 完成定时器管理
    TimerManager(EventLoop * loop, );
    ~TimerManager() = default;

    TimerID add_timer(TimeStamp when, TimerCallback cb, double interval);
    void cancle(TimerID tid);
private:
    // 处理定时器句柄的可读事件，即有定时器激活到期，内部调用tick处理到期定时器
    void handle_read(); 
    bool insert(); // 定时器列表中插入定时器
    void tick(); // 执行到期的定时器
private:
    //std::priority_queue<std::unique_ptr<Timer> > timers_;
    std::list<std::unique_ptr<Timer>> timers_;
    const int timer_fd_;
    std::unique_ptr<Channel> timer_channel_;
    EventLoop * owner_loop_;
};
}