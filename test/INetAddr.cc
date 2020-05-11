#include "../net/INetAddress.h"
#include <iostream>
using namespace conet;
using namespace std;

int main() {

    INetAddress addr1("127.0.0.1:9999");
    INetAddress addr2("127.0.0.1", 9999);
    INetAddress addr3(9999);
    INetAddress addr4(addr1);
    INetAddress addr5(":9999");

    cout << addr1.to_str() << endl;
    cout << addr2.to_str() << endl;
    cout << addr3.to_str() << endl;
    cout << addr4.to_str() << endl;

    cout << addr5.to_str() << endl;

    // INetAddress addr6("12390i"); // error

    cout << addr1.get_ip_str() << " " << addr1.get_port() << endl;
    cout << (addr1 == addr4) << endl;
    return 0;
}