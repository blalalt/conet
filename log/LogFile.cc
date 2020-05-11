#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include "LogFile.h"

using namespace conet;

File::File(std::string filename) 
    : fp_(::fopen(filename.c_str(), "w")), written_bytes_(0) {
    assert(fp_);
    ::memset(buf_, '\0', sizeof(buf_));
    ::setbuffer(fp_, buf_, sizeof(buf_));
}
void File::flush() {
    ::fflush(fp_);
}

void File::append(const char * src, size_t len) {
    size_t n = write(src, len);
    size_t remain = len - n;
    while (remain > 0) {
        size_t x = write(src+n, len);
        if (x == 0) {
            int err = ::ferror(fp_);
            if (err) {
                fprintf(stderr, "LogFile::File::append error");
            }
            break; //  TODO: 错误处理待完成
        }
        n += x; // 已经写入的总数
        remain = len - n; // 剩余待写入的
    }
    written_bytes_ += len;
}

size_t File::write(const char * src, size_t len) {
    return ::fwrite_unlocked(src, 1, len, fp_);
}

File::~File() {
    ::fclose(fp_);
}


LogFile::LogFile(std::string base_name, size_t roll_size) 
    : roll_size_(roll_size), base_filename_(base_name), cur_filename_() {
    
        roll_file();
}
std::string LogFile::get_file_name() {
    std::string fn;
    fn.reserve(base_filename_.size() + 64);
    fn = base_filename_;
    time_t tt;
    time(&tt);
    tm * tm_tm = ::gmtime(&tt);
    char timebuf[64];
    strftime(timebuf, sizeof timebuf, ".%Y%m%d-%H%M%S", tm_tm);
    fn += timebuf;
    return fn+".log";
}
void LogFile::roll_file() {
    std::string filename = get_file_name();
    if (!file_) {
        file_.reset(new File(filename));
    }
    struct stat st;
    ::fstat(fileno(file_->stream()), &st);
    if (st.st_size > roll_size_) {
        file_.reset(new File(filename));
    }
}
void LogFile::append(const char *src, int len) {
    file_->append(src, len);
    if (file_->written_bytes() > roll_size_) {
        roll_file();
    }
}
void LogFile::fulsh() {
    file_->flush();
}