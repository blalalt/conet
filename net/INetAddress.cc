#include <string.h>
#include <exception>
#include <arpa/inet.h>
#include "INetAddress.h"

using namespace conet;

INetAddress::INetAddress(const std::string ip_port) {
    auto mpos = ip_port.rfind(':');
    if (mpos == std::string::npos) throw "Bad IP:PORT format.";

    if (mpos == 0) create(std::stod(ip_port.substr(1)));
    else create(ip_port.substr(0, mpos), std::stod(ip_port.substr(mpos+1)));
}
INetAddress::INetAddress(int port) {
    create(port);
}
INetAddress::INetAddress(const std::string ip, int port) {
    create(ip, port);
}
INetAddress::INetAddress(const INetAddress & other) {
    addr_.sin_family = other.addr_.sin_family;
    addr_.sin_port = other.addr_.sin_port;
    addr_.sin_addr.s_addr = other.addr_.sin_addr.s_addr;
}

void INetAddress::create(int port) {
    addr_.sin_family = AF_INET;
    addr_.sin_port = ::htons(port);
    addr_.sin_addr.s_addr = INADDR_ANY;
}
void INetAddress::create(const std::string ip, int port) {
    addr_.sin_family = AF_INET;
    addr_.sin_port = ::htons(port);
    ::inet_pton(AF_INET, ip.c_str(), &addr_.sin_addr);
}

int INetAddress::get_port() const{ 
    return ::htons(addr_.sin_port); 
}

std::string INetAddress::to_str() const {
    std::string ip;
    int port;
    std::tie(ip, port) = for_human();
    return ip + ':' + std::to_string(port);
}

std::string INetAddress::get_ip_str() const {
    std::string ip;
    int port;
    std::tie(ip, port) = for_human();
    return ip;
}

std::pair<std::string, int> INetAddress::for_human() const {
    std::string ip(::inet_ntoa(addr_.sin_addr));
    int port = ::ntohs(addr_.sin_port);
    return std::make_pair(ip, port); // 使用make_pair 或 tuple 最好不要带模板参数
}

bool INetAddress::operator==(const INetAddress & other) {
    if (addr_.sin_family == other.addr_.sin_family &&
        addr_.sin_port   == other.addr_.sin_port   &&
        addr_.sin_addr.s_addr == other.addr_.sin_addr.s_addr)
        return true;
    return false;
}

INetAddress INetAddress::resolve_name(const std::string hostname) {
    // TODO: 主机名到IP地址的解析
}