/*
 * @Description: 
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-22 14:28:04
 */
#pragma once
#include <functional>
#include <memory>
#include "../utils/TimeStamp.h"
namespace conet {

class EventLoop;

class Channel {
public:
    typedef std::function<void()> EventCallback;
    typedef std::function<void(TimeStamp)> ReadEventCallback;
    typedef int handle_t;

    Channel(EventLoop * loop, handle_t fd);
    ~Channel() = default;

    void handle_event(TimeStamp recive_time);
    void set_read_cb(const ReadEventCallback & cb) {
        read_cb_ = cb;
    }
    void set_write_cb(const EventCallback & cb) {
        write_cb_ = cb;
    }
    void set_close_cb(const EventCallback & cb) {
        close_cb_ = cb;
    }
    void set_error_cb(const EventCallback & cb) {
        error_cb_ = cb;
    }

    // 为了在Channel::handleEvent处理期间，防止因其owner对象被修改，
    // 进而导致Channel被析构，最后出现不可预估错误。
    // Channel::tie()的作用就是将Channel的owner对象进行绑定保护起来。
    void tie(std::shared_ptr<void> & owner);
    handle_t fd() const { return fd_; }
    int events() const { return event_; }
    void set_revents(int revt) { revent_ = revt; }
    bool is_none_events() const { return event_ == kNoneEvent ;}
    void enable_read() { event_ |= kReadEvent; update(); }
    void enable_write() { event_ |= kWriteEvent; update(); }
    void disable_write() { event_ &= ~kWriteEvent; update(); }
    void disable_all() { event_ = kNoneEvent; update(); }
    // TODO: 完善 Channel 的相关函数
    // remove的作用是删除channel在Poll中的地址拷贝，而不是销毁channel。
    // channel的销毁由其owner对象决定。
    void remove();
    // 设置 Channel 的状态
    int status() const { return status_; }
    void set_status(int st) { status_ = st; }

private:
    void update(); // 更新注册事件
    void handle_event_with_guard(TimeStamp);

private:

    static const int kNoneEvent;
    static const int kWriteEvent;
    static const int kReadEvent;

    int event_; // 通道注册的事件
    int revent_; // 通道产生的事件，由Poller填充
    int status_; // 通道的状态
    bool tied_;
    std::weak_ptr<void> tie_; // Channel的owner对象的引用
    handle_t fd_; // 通道相关的文件句柄
    EventLoop * loop_; // 通道所属的IO线程 事件循环

    ReadEventCallback read_cb_;
    EventCallback write_cb_;
    EventCallback close_cb_;
    EventCallback error_cb_;
};

}