/*
 * @Description:
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-11 17:37:17
 */

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
    LOG_INFO("%s", "He");
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
    Logger::set_level(Logger::LogLevel::DEBUG);
    LOG_INFO("%s:%d", "func.main", __LINE__);
//    bench(10000000);
    return 0;
}