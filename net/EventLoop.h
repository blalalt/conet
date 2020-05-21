/*
 * @Description: 事件循环，Reactor
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-21 20:46:02
 */ 
#include <memory>
#include <vector>
#include <thread>
#include <mutex>

namespace conet {

class Channel;
class TimerManager;
class Poller;

class EventLoop {
public:

private:
    typedef int handle_t;
    typedef std::function<void()> Functor;
    typedef std::vector<std::unique_ptr<Channel> > ChannelList;
    // 四个状态值
    bool quit_; // 是否退出
    bool looping_; // 是否结束事件循环
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