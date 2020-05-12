/*
 * @Description: 
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-11 15:23:18
 */
#pragma once
#include <vector>
#include <memory>
#include "INetAddress.h"
namespace conet
{
    class Socket;
    typedef std::shared_ptr<Socket> SocketPtr;

    class Socket // non copyable
    {
    protected:
        const static int INVALID_SOCKET = -1;
    public:
        Socket() : handle_(INVALID_SOCKET) {}
        explicit Socket(int sockfd) : handle_(sockfd) {}
        ~Socket() { if (handle_ != INVALID_SOCKET) close(); }
        Socket(Socket &&sock) noexcept : handle_(sock.handle_) {
            sock.handle_ = INVALID_SOCKET;
        }
        Socket(const Socket &) = delete;
        Socket& operator=(const Socket&) = delete;

        void bind(const INetAddress &addr);
        bool is_valid() const;
        void close(); // 同时关闭读写
        void shutdown_write(); // 关闭读
        int handle() const { return handle_; }
        void set_non_block();
        void set_reuse_addr(bool on);
        // INetAddress get_addr();
        int set_option(int level, int opt_name, void *val, socklen_t len);
        int get_option(int level, int opt_name, void *val, socklen_t len) const;

        ssize_t write(void *buf, size_t count);
        ssize_t read(void *buf, size_t count);
        ssize_t write_v(std::vector<iovec> &vecs);
        ssize_t read_v(std::vector<iovec> &vecs);
        
        bool operator==(const Socket &);

        static Socket create(int domain, int type, int protocol=0);
        static std::tuple<Socket, Socket> pair(int domain, int type, int protocol=0);

    private:
        int handle_;
    };

} // namespace conet