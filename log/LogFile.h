#pragma once
#include <stdio.h>
#include <string>
#include <memory>
namespace conet {

// 对 FILE 的封装
class File {

public:
    File(std::string filename);
    ~File();
    void append(const char * src, size_t len);
    FILE * stream() { return fp_; }
    size_t written_bytes() const { return this->written_bytes_; }
    void flush();
private:
    size_t write(const char * src, size_t len); // 内部的真正写到文件内，append只是写到缓冲区
private:
    char buf_[10 * 1024]; // 写入的缓冲
    FILE * fp_; // 文件指针
    size_t written_bytes_; // 已经写入的字节数
};

// 日志文件的抽象
class LogFile {
public:
    LogFile(std::string base_name, size_t roll_size);
    ~LogFile() = default;
    std::string get_file_name(); // 获取新的文件的名字
    void roll_file(); // 如果文件大小达到限制的话就新建文件并充值 file_
    void append(const char *src, int len);
    void fulsh();
private:
    size_t roll_size_; // 文件的大小，超过这个大小就新建另一个文件
    std::unique_ptr<File> file_;
    std::string base_filename_;
    std::string cur_filename_;
};




}
