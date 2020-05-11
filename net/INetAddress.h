/*
 * @Description: 
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-11 17:12:23
 */
#pragma once

#include <string>
#include <tuple>
#include <netinet/in.h>

namespace conet {

class INetAddress {
public:
    explicit INetAddress(const std::string ip_port);
    explicit INetAddress(int port);
    INetAddress(const std::string ip, int port);
    INetAddress(const INetAddress & other);


    sockaddr* ptr() { return reinterpret_cast<sockaddr*>(&addr_); }
    // sockaddr* ptr() { return const_cast<sockaddr*>(ptr()); }

    socklen_t length () const { return sizeof(addr_); }
    sa_family_t family() const { return addr_.sin_family; }
    std::string to_str() const;
    std::string get_ip_str() const;

    int get_port() const;

    bool operator==(const INetAddress & other);

    static INetAddress resolve_name(const std::string hostname);
private:
    void create(int port);
    void create(const std::string ip, int port);
    std::pair<std::string, int> for_human() const;
    sockaddr_in addr_;
};

};
