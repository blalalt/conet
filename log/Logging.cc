/*
 * @Description: 
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-13 15:01:59
 */
#include <functional>
#include "Logging.h"

using namespace conet;


void FixedBuffer::append(const char * src, int len) {
    if (avail() > len) {
        memcpy(cur_, src, len);
        cur_ += len;
    } else {
        memcpy(cur_, src, avail());
        cur_ += avail();
    }
}
Logging::Logging(std::string filename, size_t roll_size, int flush_interval)
    : file_(filename, roll_size), 
      stop_(false),
      cur_buf_(new FixedBuffer),
      flush_interval_(flush_interval)
{
    buffers_.reserve(16);
    start();
}

Logging::~Logging() {
    if (!stop_) {
        stop();
    }
}
void Logging::append(const char * src, int len) {
    std::unique_lock<std::mutex> lck(mutex_);
    // 往缓冲队列内添加日志
    // 如果当前的缓冲区空间还足够的话，直接使用当前缓冲区
    if (cur_buf_->avail() > len) {
        cur_buf_->append(src, len); // 这里先不直接唤醒写入线程
    } else {
        // 空间不足
        // 把当前缓冲区加入到缓冲队列
        buffers_.push_back(std::move(cur_buf_));
        // 新建一个缓冲区
        cur_buf_ = std::make_unique<FixedBuffer>();
        cur_buf_->append(src, len);
        cond_.notify_one(); // 唤醒写入线程
    }
}

void Logging::thread_func() {
//    fprintf(stdout, "%s: %s\n", "in thread func.", std::this_thread::get_id());
//    std::cout << "id=" << std::this_thread::get_id() << std::endl;
    std::vector<BufferPtr> bufs_to_write;
    bufs_to_write.reserve(16); // 先预留空间
    while (!stop_) {
        {

            std::unique_lock<std::mutex> lck(mutex_);
            cond_.wait(lck, [this]{ return !buffers_.empty();});
            // if (buffers_.empty()) {
//                cond_.wait_for(lck, std::chrono::seconds(flush_interval_));
                // cond_.wait(lck);
            // }
//            lck.unlock();
//            sleep(100);
            // 把当前缓冲的内容添加到缓冲队列(待写入)中
            buffers_.push_back(std::move(cur_buf_));
            cur_buf_ = std::make_unique<FixedBuffer>();
            // 把缓冲队列转移到写入队列中
            std::swap(buffers_, bufs_to_write);
        }

       // 写入到 LogFile 中
        for (int i=0; i<bufs_to_write.size(); i++) {
            file_.append(bufs_to_write[i].get()->data(), bufs_to_write[i].get()->length());
        }
        file_.fulsh();
    }
    file_.fulsh();
}

void Logging::start() {
    stop_ = false;
    thread_ = std::thread(std::bind(&Logging::thread_func, this));
}



void Logging::stop() {
    stop_ = true;
    cond_.notify_all(); // 唤醒线程，防止其还在队列上阻塞
    thread_.join();
}