/*
 * @Description: 
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-11 15:48:20
 */
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/uio.h>
#include "Socket.h"
#include "../log/Logger.h"

using namespace conet;


void Socket::bind(const INetAddress &addr) {
    int status = ::bind(handle_, addr.ptr(), addr.length());
    if (status < 0) {
        int err = errno;
        LOG_FATAL("%s", ::strerror(err));
    }
}
bool Socket::is_valid() const {
    return handle_ == INVALID_SOCKET;
}
void Socket::close() {
    int status = ::close(handle_);
    if (status < 0) LOG_ERROR("socket.close error.");
}
void Socket::shutdown_write() {
    int status = ::shutdown(handle_, SHUT_WR);
    if (status < 0) LOG_ERROR("socket.shutdown error.");
}

bool Socket::operator==(const Socket &other) {
    return handle_ == other.handle_;
}

void Socket::set_non_block() {
    int old_opt = ::fcntl(handle_, F_GETFL);
    int new_opt = old_opt | O_NONBLOCK;
    ::fcntl(handle_, F_SETFL, new_opt); 
}

void Socket::set_reuse_addr(bool on) {
    int val = on ? 1 : 0;
    set_option(SOL_SOCKET, SO_REUSEADDR, &val, sizeof(val));
}
int Socket::set_option(int level, int opt_name, void *val, socklen_t len) {
    ::setsockopt(handle_, level, opt_name, val, len);
}
int Socket::get_option(int level, int opt_name, void *val, socklen_t len) const {
    ::getsockopt(handle_, level, opt_name, val, &len);
}
ssize_t Socket::write(void *buf, size_t count) {
    return ::write(handle_, buf, count);
}
ssize_t Socket::read(void *buf, size_t count) {
    return ::read(handle_, buf, count);
}
ssize_t Socket::write_v(std::vector<iovec> &vec) { 
    return ::writev(handle_, vec.data(), vec.size());
}
ssize_t Socket::read_v(std::vector<iovec> &vec) {
    return ::readv(handle_, vec.data(), vec.size());
}

Socket Socket::create(int domain, int type, int protocol) {
//    return std::make_shared<Socket>(::socket(domain, type, protocol));
    return Socket(::socket(domain, type, protocol));
}
std::tuple<Socket, Socket> Socket::pair(int domain, int type, int protocol) {
    int fds[2] = {0};
    socketpair(domain, type, protocol, fds);
//    return std::make_tuple( std::make_shared<Socket>(fds[0]),
//                            std::make_shared<Socket>(fds[1]));
    return std::make_tuple(Socket(fds[0]), Socket(fds[1]));
}