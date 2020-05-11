
#include "../log/Logger.h"
#include "../log/Logging.h"
#include <unistd.h>
using namespace conet;

Logging g_log("log", 4 * 1024, 3);

void bench(int times) {
    for (int i=0; i<times; i++) {
        ::fprintf(stdout, "%d\n", i);
        LOG_INFO("1");
        sleep(1);
    }
}

int main () {
    Logger::set_output_func([](const char * src, int len) {
        ::fprintf(stdout, "%s\n", src);
        g_log.append(src, len);
    });
    Logger::set_flush_func([]() { return ; });
    sleep(1);
    LOG_INFO("Hello World.");
    LOG_INFO("Hello World.");
    Logger::set_level(Logger::LogLevel::WARN);
    LOG_INFO("Hello World.");
    LOG_INFO("Hello World.");
//    bench(10000000);
    return 0;
}