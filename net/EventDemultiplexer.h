/*
 * @Description: 事件多路分发器
 * @version: 
 * @Author: blalalt
 * @Date: 1970-01-01 08:00:00
 */
#pragma once
#include <vector>
#include <map>

namespace conet
{
    class Channel;
    class EventLoop;
    class EventDemultiplexer
    {
    public:
        EventDemultiplexer(EventLoop *loop);
        ~EventDemultiplexer();
        // IO复用函数
        void demultiplex(const int timeoutms,
                         std::vector<Channel *> out_channels);
        void updateChannel(Channel *channel);
        void removeChannel(Channel *channel);

    private:
        static const int kListenEventListSize = 16;
        typedef int handle_t;
        typedef std::vector<Channel *> ChannelList;
        typedef std::map<handle_t, Channel *> ChannleMap;
        EventLoop *this_loop_;
        handle_t epoll_fd_;          // epoll 句柄
        ChannelList events_;        // 监听的兴趣列表
        ChannleMap handle2channel_; // fd到channel映射
    };
} // namespace conet