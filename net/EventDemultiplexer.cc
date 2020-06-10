/*
 * @Description: 
 * @version: 
 * @Author: blalalt
 * @Date: 1970-01-01 08:00:00
 */ 

#include "EventDemultiplexer.h"
#include <sys/epoll.h>

using namespace conet;

EventDemultiplexer::EventDemultiplexer(EventLoop * loop)
  : this_loop_(loop),
    epoll_fd_(::epoll_create1(EPOLL_CLOEXEC)),
    events_(kListenEventListSize)
{

}

EventDemultiplexer::~EventDemultiplexer() {

}

EventDemultiplexer::updateChannel() {

}
EventDemultiplexer::removeChannel() {

}
EventDemultiplexer::demultiplex() {
    
}