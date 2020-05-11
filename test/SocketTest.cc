/*
 * @Description: 
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-11 16:55:17
 */

#include "../net/INetAddress.h"
#include "../net/Socket.h"
#include "../log/Logger.h"
#include <iostream>
using namespace conet;
using namespace std;
int main() {
    LOG_INFO("start!"); 
    Socket s1;
    cout << "is_valid: " << s1.is_valid() << endl;

    Socket s2 = Socket::create(AF_INET, SOCK_STREAM, 0);
    cout << "fd: " << s2.handle() << endl;
    INetAddress addr("127.0.0.1:9999");
    s2.bind(addr); 

    Socket sig_read, sig_write;
    std::tie(sig_read, sig_write) = Socket::pair(AF_UNIX, SOCK_STREAM, 0);
    char write_buf[1024];
    snprintf(write_buf, sizeof(write_buf), "%s\n", "hello world.");
    sig_write.write(write_buf, sizeof(write_buf));

    char read_buf[1024];    
    sig_read.read(read_buf, sizeof(read_buf));
    cout << "recive data: " << read_buf << endl;
    return 0;
}