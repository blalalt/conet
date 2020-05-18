/*
 * @Description: 缓冲区类
 * @version: 作为收发数据的缓冲区，减少系统调用，处理不完整消息
 * @Author: blalalt
 * @Date: 2020-05-16 17:47:00
 */ 


#pragma once
#include <vector>
#include <string>
namespace conet {


class Buffer {
public:
    Buffer();
    size_t readable_bytes() const { return write_idx_ - read_idx_; }
    const char * peek() const { return &data_[read_idx_]; }
    void append(const void * src, size_t len);
    void prepend(const void * src, size_t len);
    void retrieve(size_t len);
    std::string retrieve_as_string(size_t len);
private:
    int read_from_fd(size_t len);
    size_t writable_bytes() const { return data_.size() - write_idx_; }
    void check_space(size_t len);

private:
    const static int kInitialSize = 1024;
    const static int kCheapPrepend = 8;
    const static int kMaxSize = 10 * 1024; // 10KB
    std::vector<char> data_;
    int read_idx_;
    int write_idx_;
};


Buffer::Buffer() : read_idx_(kCheapPrepend), write_idx_(kCheapPrepend) {}

void Buffer::append(const void * src, size_t len) {

}

void Buffer::prepend(const void * src, size_t len) {

}

void Buffer::retrieve(size_t len) {

}

std::string Buffer::retrieve_as_string(size_t len) {

}

void Buffer::check_space(size_t len) {
    if (len <= writable_bytes()) return ;
    data_.resize(data_.size() + len);
}

}