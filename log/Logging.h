#pragma once
#include <string.h>
#include <string>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <memory>

#include "LogFile.h"

namespace conet
{
    class FixedBuffer;
    typedef std::unique_ptr<FixedBuffer> BufferPtr;

    const int k_buffer_size = 4 * 1024;
    class FixedBuffer {
        public:
            FixedBuffer() : cur_(buf_) { clear(); }
            char * current() { return cur_; } // 当前指针
            int length() const { return static_cast<int>(cur_ - buf_); }
            const char * data () { return buf_; } // 缓冲区内的内容
            int avail() { return static_cast<int>(end() - cur_); } // 当前可用空间
            void clear() { memset(buf_, '\0', k_buffer_size); cur_ = buf_; } // 清空缓冲区
            void append(const char * src, int len); // 添加到缓冲区
        private:
            const char * end() { return buf_ + sizeof(buf_); }
            char buf_[k_buffer_size]; // 缓冲区
            char * cur_; // 当前指针，缓冲区内的位置
    };

    class Logging {
        public:
            Logging(std::string filename, size_t roll_size, int flush_interval);
            ~Logging();
            void start(); // 开始
            void stop(); // 停止
            void append(const char * src, int len);
        private:
            void thread_func();
        private:
            std::thread thread_; // 输出的线程
            std::mutex mutex_; // 其他线程往缓冲队列写的时候要控制访问次序
            std::condition_variable cond_;

            bool stop_; // 是否停止
            int flush_interval_;

            // 缓冲队列，其他线程往这里写入，
            // thread_func_ 把缓冲队列中的内容写入到 file_
            std::vector<BufferPtr> buffers_; 
            BufferPtr cur_buf_; // 指向当前缓冲队列中的元素

            LogFile file_; // 输出到日志文件
    };
}