/*
 * @Description: 事件循环，Reactor, at most one per thread.
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-21 20:46:02
 */ 
#include <memory>
#include <vector>
#include <thread>
#include <mutex>
#include "Timer.h"
#include "../utils/TimeStamp.h"

namespace conet {

class Channel;
class TimerManager;
class Poller;

class EventLoop {
public:
    EventLoop();
    ~EventLoop();
    void wakeup(); // 唤醒
    bool is_inloop_thread()  { return thread_id_ == std::this_thread::get_id(); }
    void assert_inloop_thread() {
        if (!is_inloop_thread()) {
            abort_not_inloop_thread();
        }
    }
    // 事件循环相关函数
    void loop();
    void quit();
        // 线程安全的，因为只会在当前IO线程内执行,
        // 但对 pending_functors_的访问是不安全的，所以需要加锁
    void run_inloop(const Functor& cb);
    void queue_inloop(const Functor& cb);

    // 通道相关函数
    void update_channel(Channel * channel);
    void remove_channel(Channel * channel);

    // 定时器相关函数
    TimerId register_timer(Timer timer);
    void unregister_timer(TimerID tid);
private:
    // 函数内调用read 掉 wakeupFd_ 的数据，避免一直触发。
    void handle_read(); // wake up
    void abort_not_inloop_thread();
private:
    typedef int handle_t;
    typedef std::function<void()> Functor;
    // 这里不控制 Channel的生命周期
    typedef std::vector<Channel*> ChannelList;
    // 四个状态值
    bool quit_; // 是否退出
    bool looping_; // 是否正在运行事件循环
    bool doing_pending_functors_; // 是否正在执行 pending functors
    bool event_handling_; // 是否正在处理事件

    // 当前事件循环所在的线程，控制每个线程仅有一个EventLoop
    std::thread::id thread_id_;

    // 唤醒当前EventLoop的通道(将一个通道可写，Poll会返回)
    handle_t wakeup_fd_;
    std::unique_ptr<Channel> wakeup_channel_;

    // 其他任务，别的线程发送过来的，让当前IO线程执行
    std::vector<Functor> pending_functors_; // Guard by mutex_
    std::mutex mutex_;

    // Poll返回的活跃通道
    ChannelList active_channels_;

    // IO复用
    std::unique_ptr<Poller> poll_;
    // 定时器管理
    std::unique_ptr<TimerManager> timer_queue_;
};

}