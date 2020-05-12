/*
 * @Description: 
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-11 22:53:58
 */
#include <iostream>
#include "../net/Acceptor.h"
using namespace conet;
using namespace std;
int main() {

    Socket sock = Socket::create(AF_INET, SOCK_STREAM, 0);
    sock.set_reuse_addr(true);
    INetAddress addr("127.0.0.1:9999");

    Acceptor acceptor(std::move(sock), addr);
    acceptor.listen();

    while (true) {
        INetAddress client_addr;
        Socket client_sock = acceptor.accept(client_addr);
        cout << client_addr.to_str() << endl;
    }

    return 0;
}