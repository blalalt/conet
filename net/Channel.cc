//
// Created by Administrator on 2020/5/23.
//

#include "Channel.h"
#include "EventLoop.h"
#include <sys/epoll.h>
using namespace conet;

const int Channel::kNoneEvent = 0;
const int Channel::kWriteEvent = EPOLLOUT;
const int Channel::KReadEvent = EPOLLIN | EPOLLPRI;

Channel::Channel(conet::EventLoop *loop, conet::Channel::handle_t fd)
    : loop_(loop),
      fd_(fd),
      tied_(false),
      event_(0),
      revent_(0),
      status(-1)
{

}

Channel::~Channel() {

}

void Channel::tie(std::shared_ptr<void> & owner) {
    // 如果持有者可能会在Channel::handleEvent()执行到一半的时候，执行析构
    // 其所属的Channel对象本身被销毁了
    // 这个函数，用于延长某些对象（可以是Channel对象，也可以是其owner对象）的生命期，
    // 使之长过Channel::handleEvent()函数。
    // 该函数由Channel持有者调用，当其认为可能在处理的时候会关闭Channel时
    tie_ = owner;
    tied_ = true;
}


void Channel::update() {
    loop_->update_channel(this);
}

void Channel::remove() {
    loop_->remove_channel(this);
}

void Channel::handle_event(TimeStamp recive_time) {
    if (tied_) {
        // 持有对象，拉一拉绳子，看对象是否还存在
        std::shared_ptr<void> guard = tie_.lock();
        if (guard) {
            handle_event_with_guard(recive_time);
        }
    }
    else {
        handle_event_with_guard(recive_time);
    }
}

void Channel::handle_event_with_guard(TimeStamp stamp) {
    // 处理各种事件
    if ((revent_ & EPOLLHUP) && !(revent_ & EPOLLOUT)) {
        if(close_cb_) close_cb_();
    }
    if (revent_ & EPOLLIN) {
        if (read_cb_) read_cb_(stamp);
    }
    if (revent_ & EPOLLOUT) {
        if (write_cb_) write_cb_();
    }
    if (revent_ & (EPOLLERR | POLLNVAL) {
        if (error_cb_) error_cb_();
    }
}