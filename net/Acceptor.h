/*
 * @Description: 
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-11 21:12:30
 */
#pragma once

#include "Socket.h"

namespace conet
{

class Acceptor {
public:
    Acceptor(Socket sock, const INetAddress & addr);
    Acceptor(const INetAddress & addr);
    Acceptor(const Acceptor &) = delete;
    Acceptor& operator==(const Acceptor&) = delete; 
    ~Acceptor();
    void listen();
    bool listenning() const;
    Socket accept(INetAddress & addr);
private:
    Socket sock_;
    int idle_fd_; // 预留一个空闲的文件描述符，防止EMFILE错误
    bool is_listenning; // 是否正在监听
};



}