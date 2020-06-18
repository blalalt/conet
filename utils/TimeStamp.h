/*
 * @Description: 
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-22 09:08:56
 */ 
#pragma once
#include <sys/time.h>
#include <cstdint>

namespace conet {

class TimeStamp {

public:
    TimeStamp() : micro_sec_since1970(0) {}
    explicit TimeStamp(int64_t micro_sec)
     : micro_sec_since1970(micro_sec) {}

    bool is_valid() const { return micro_sec_since1970 > 0; }
    int64_t micro_sec() const { return micro_sec_since1970; }
    time_t sec() const {
        return static_cast<time_t>(micro_sec_since1970 / kMicroPerSecond);
    }

    static const int kMicroPerSecond = 1e6;
    static TimeStamp now() {
        struct timeval tv;
        ::gettimeofday(&tv, nullptr);
        int64_t sec = tv.tv_sec;
        return TimeStamp(sec * TimeStamp::kMicroPerSecond + tv.tv_usec);
    }
    static TimeStamp invalid() {
        return TimeStamp();
    }
    bool operator<(const TimeStamp & other) const {
        return this->micro_sec_since1970 < other.micro_sec_since1970;
    }
    bool operator>(const TimeStamp & other) const {
        return this->micro_sec_since1970 > other.micro_sec_since1970;
    }
    bool operator<=(const TimeStamp & other) const {
        return this->micro_sec_since1970 <= other.micro_sec_since1970;
    }
    bool operator>=(const TimeStamp & other) const {
        return this->micro_sec_since1970 >= other.micro_sec_since1970;
    }
    bool operator==(const TimeStamp & other) const {
        return this->micro_sec_since1970 == other.micro_sec_since1970;
    }
private:
    int64_t micro_sec_since1970;
};

//TimeStamp TimeStamp::now()
//
//TimeStamp TimeStamp::invalid() {
//    return TimeStamp();
//}

inline TimeStamp operator-(const TimeStamp high, const TimeStamp low) {
    int64_t diff = high.micro_sec() - low.micro_sec();
    return TimeStamp(diff);
}

inline TimeStamp operator+(const TimeStamp t, double sec) {
    int64_t delta = static_cast<int64_t>(sec * TimeStamp::kMicroPerSecond);
    return TimeStamp(delta + t.micro_sec());
}
inline TimeStamp operator+(double sec, const TimeStamp t) {
    int64_t delta = static_cast<int64_t>(sec * TimeStamp::kMicroPerSecond);
    return TimeStamp(delta + t.micro_sec());
}


}