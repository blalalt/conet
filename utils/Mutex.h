/*
 * @Description: 按照C++ mutex封装这些锁，这样可以统一接口并且可以
 *               使用一些c++thread中的模板，如lock_guard
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-13 15:29:39
 */
#pragma once 

#include <pthread.h>

namespace conet {

class Spinlock { // 适用于短时间内占用锁的情景，效率高于互斥量
public:
    typedef pthread_spinlock_t native_handle_type;
    Spinlock() {
        pthread_spin_init(&lock_, 0);
    }

    native_handle_type native_handle() const { return lock_; }

    ~Spinlock() {
        pthread_spin_destroy(&lock_);
    }

    void lock() {
        pthread_spin_lock(&lock_);
    }

    void unlock() {
        pthread_spin_unlock(&lock_);
    }
private:
    pthread_spinlock_t lock_;
};

class RWlock {
    RWlock() {
        pthread_rwlock_init(lock_, nullptr);
    }
    ~RWlock() {
        pthread_rwlock_destroy(lock_);
    }
    void rdlock() {
        pthread_rwlock_rdlock(lock_);
    }

    void wrlock() {
        pthread_rwlock_wrlock(lock_);
    }

    void unlock() {
        pthread_rwlock_unlock(lock_);
    }
private:
    pthread_rwlock_t lock_;
};

class Readlock {
public:
    Readlock(RWlock & lock): lock_(lock) {}
    typedef pthread_rwlock_t native_handle_type;
    void lock() {
        lock_.rdlock();        
    }
    void unlock() {
        lock_.unlock();
    }
private:
    RWlock & lock_;
};
class Writelock{
public:
    Writelock(RWlock & lock): lock_(lock) {}
    typedef pthread_rwlock_t native_handle_type;
    void lock() {
        lock_.rdlock();        
    }
    void unlock() {
        lock_.unlock();
    }
private:
    RWlock & lock_;
};

}