#include <sstream>
#include <chrono>
#include <assert.h>
#include "Logger.h"

using namespace conet;

static const char* log_level_names[Logger::NUM_OF_LEVEl] = {
    "TRACE",
    "DEBUG",
    "INFO ",
    "WARN ",
    "ERROR",
    "FATAL"
};

Logger::LogLevel g_log_level = Logger::LogLevel::DEBUG;

std::function<void(const char *, int)> Logger::out_put_func_ = [] (const char *src, int len) {
    ::fwrite(src, 1, len, stdout);
};
std::function<void()> Logger::flush_func_ = [] () {
    ::fflush(stdout);
};


void Logger::set_output_func(std::function<void(const char*, int)> func) {
    out_put_func_ = func;
}
void Logger::set_flush_func(std::function<void()> func) {
    flush_func_ = func;
}
void Logger::set_level(LogLevel level) {
    g_log_level = level;
}

thread_local time_t t_last_second;
thread_local char t_buf[24];
std::string Logger::format_time() {
    using std::chrono::time_point_cast;
    using std::chrono::microseconds;
    using std::chrono::system_clock;
    using std::chrono::system_clock;

    size_t day_len = 19;

    auto tp = time_point_cast<microseconds>(system_clock::now());
    int64_t ms = tp.time_since_epoch().count();
    int millsec = ms % 1000000;
    time_t tt = static_cast<time_t>(ms / 1000000);
    // 如果距离上次写入日志还不足一秒，就不需要重新计算日期了，只需毫秒数即可
    if (tt != t_last_second) {
        t_last_second = tt;
        struct tm tm_time; // 转换为 utc 时间
        ::gmtime_r(&tt, &tm_time);
        day_len = snprintf(t_buf, 22, "%4d-%02d-%02d %02d:%02d:%02d",
                          tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
                          tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);
    }
    assert(day_len == 19);
    snprintf(t_buf+day_len, 5, ".%03d", millsec);
    return std::string(t_buf);
}
std::string Logger::format_file(const std::string file_name) {
    auto pos = file_name.rfind('/');
    if (pos == std::string::npos) return file_name;
    else return file_name.substr(pos+1);
}

void Logger::log(LogLevel level, const char *file, int line, std::string msg){
    // 输出的格式为 [时间][日志等级] (所在文件:行号) - 输出的内容
    // [2019-02-12 09:22:10:3][INFO] (LogFile.cc:35) - 启动
    // char out_buf[1024];
    // ::snprintf()
    if (level < g_log_level) return ;
    std::ostringstream os;
    os << '[' << format_time() << ']';
    os << '[' << log_level_names[level] << ']';
    os << '(' << format_file(file) << ':' << line << ')';
    os << " - " << msg << '\n';
    out_put_func_(os.str().c_str(), os.str().length());
}