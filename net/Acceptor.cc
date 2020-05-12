/*
 * @Description: 
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-11 22:03:31
 */
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "Acceptor.h"
#include "../log/Logger.h"
using namespace conet;
const static int QUEUE_SIZE = 10;
Acceptor::Acceptor(Socket sock, const INetAddress & addr)
    : is_listenning(false),
      sock_(std::move(sock)),
      idle_fd_(::open("/dev/null", O_RDONLY | O_CLOEXEC))
{
    sock_.bind(addr);

}

Acceptor::Acceptor(const INetAddress & addr)
    : is_listenning(false),
      sock_(Socket::create(AF_INET, SOCK_STREAM, 0)),
      idle_fd_(::open("/dev/null", O_RDONLY | O_CLOEXEC))
{
    sock_.bind(addr);
}

Acceptor::~Acceptor() {
    ::close(idle_fd_);
}

void Acceptor::listen() {
    int status = ::listen(sock_.handle(), QUEUE_SIZE);
    is_listenning = true;
}

bool Acceptor::listenning() const {
    return is_listenning;
}
Socket Acceptor::accept(INetAddress &addr) {
    socklen_t len = addr.length();
    int connfd = ::accept(sock_.handle(), addr.ptr(), &len); 
                        //    SOCK_NONBLOCK | SOCK_CLOEXEC);
    if (connfd < 0) {
        LOG_ERROR("Acceptor::accept error: %s", ::strerror(errno));
        if (errno == EMFILE) {
            ::close(idle_fd_); // 关闭预留的文件描述符，
            // 此时会有新的文件描述符可用, 接受客户端发来的连接请求
            idle_fd_ = ::accept(sock_.handle(), NULL, NULL);
            ::close(idle_fd_); // 主动断开, 否则客户端会认为一直处于连接状态
            idle_fd_ = ::open("/dev/null", O_RDONLY | O_CLOEXEC);
        }
    }
    return Socket(connfd);
}