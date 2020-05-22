/*
 * @Description: 
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-22 10:14:13
 */ 
#include <sys/eventfd.h>
#include "EventLoop.h"
#include "../log/Logger.h"

using namespace conet;

namespace {
    thread_local EventLoop * t_loop = nullptr;
    int create_eventfd() {
        int evtfd = ::eventfd(0, EFD_NONBLOCK | EFD_CLOEXEC);
        if (evtfd < 0)
        {
            LOG_SYSERR("Failed in eventfd");
            abort();
        }
        return evtfd;
    }
}

EventLoop::EventLoop()
    : quit_(false),
      looping_(false),
      doing_pending_functors_(false),
      event_handling_(false),
      thread_id_(std::this_thread::id()),
      wakeup_fd_(::create_eventfd()),
      wakeup_channel_(new Channel(this, wakeup_fd_)),
      timer_queue_(new TimerManager(this)),
      poll_(new EpollPoller(this)) 
{
    LOG_TRACE("EventLoop created %p in thread %d ", 
        this, static_cast<int>(std::this_thread::get_id()));
    
    if (t_loop) { // one thread at most one EventLoop
        LOG_FATAL("Another EventLoop in this thread.");
    }
    else {
        t_loop = this;
    }
    wakeup_channel_.set_read_cb(std::bind(&EventLoop::handle_read, this));
    wakeup_channel_.enable_read();
}

EventLoop::~EventLoop() {
    ::close(wakeup_fd_);
    t_loop = nullptr;
}