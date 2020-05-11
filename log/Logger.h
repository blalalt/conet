#pragma once
#include <functional>
#include <stdarg.h>

namespace conet
{

    class Logger
    {
    public:
        enum LogLevel
        {
            TRACE,
            DEBUG,
            INFO,
            WARN,
            ERROR,
            FATAL,
            NUM_OF_LEVEl
        };
        static void log(LogLevel level, const char *file, int line, const char *format, ...);

        

        static void set_output_func(std::function<void(const char*, int)> func); // 设置输出函数，怎么输出
        static void set_flush_func(std::function<void()> func);  // 设置刷新缓冲区的函数
        static void set_level(LogLevel level);                   // 设置日志的输出等级，小于这个等级的不输出
        static std::string format_time();                        // 格式化时间
        static std::string format_file(std::string filename);                        // 格式化文件名
        static std::function<void(const char *, int)> out_put_func_;
        static std::function<void()> flush_func_;
    };
// 下面是一些宏，在使用的时候都是使用这些宏展开，方便记录 所在文件，函数，和行数
#define LOG_INFO(format, ...)  Logger::log(Logger::INFO,  __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) Logger::log(Logger::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_TRACE(format, ...) Logger::log(Logger::TRACE, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_WARN(format, ...)  Logger::log(Logger::WARN,  __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) Logger::log(Logger::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__)
#define LOG_FATAL(format, ...) Logger::log(Logger::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__)
} // namespace conet