/*
 * @Description: 
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-22 14:28:04
 */ 
#include "../utils/TimeStamp.h"
namespace conet {

class EventLoop;

class Channel {
public:
    typedef std::function<void()> EventCallback;
    typedef std::function<void(TimeStamp)> ReadEventCallback;
    typedef int handle_t;

    Channel(EventLoop * loop, handle_t fd);
    ~Channel();

    void handle_event(TimeStamp recive_time);
    void set_read_cb(const ReadEventCallback & cb);
    void set_write_cb(const EventCallback & cb);
    void set_close_cb(const EventCallback & cb);
    void set_error_cb(const EventCallback & cb);

    // 为了在Channel::handleEvent处理期间，防止因其owner对象被修改，
    // 进而导致Channel被析构，最后出现不可预估错误。
    // Channel::tie()的作用就是将Channel的owner对象进行绑定保护起来。
    void tie(std::shared_ptr<void> & );
    handle_t fd() const { return fd_; }
    int events() const { return event_; }
    void set_revents(int revt) { revent_ = revt; }
    
private:
    int event_; // 通道注册的事件
    int revent_; // 通道产生的事件，由Poller填充
    int status_; // 通道的状态
    bool tied_;
    handle_t fd_; // 通道相关的文件句柄
    EventLoop * loop_; // 通道所属的IO线程 事件循环

    ReadEventCallback read_cb_;
    EventCallback write_cb_;
    EventCallback close_cb_;
    EventCallback error_cb_;
};
};

}