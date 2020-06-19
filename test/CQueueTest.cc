/*
 * @Description: 
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-14 18:45:39
 */

#include "../ds/ConcurrentQueue.h"
#include <iostream>
#include <thread>
#include <vector>
#include <assert.h>
#include <functional>
#include <unistd.h>
using namespace conet;
using namespace std;

ConcurrentQueue<int> q;

void put_to_q() {
    q.put(1);
}

void take_to_q() {
    int a ;
    q.take(a);
    // cout << showSuggestions << endl;
}

void takeq1(ConcurrentQueue<int> & q1) {
    int a;
    q1.take(a);
}

// 测试：一个线程阻塞在队列上时，队列被销毁了，会发生什么情况
void test_when_wait_destory() {
    thread t;
    {
        ConcurrentQueue<int> q1;
        t = thread(std::bind(takeq1, std::ref(q1)));
        sleep(1);
        // q1 销毁
    }
    t.join();
    cout << "down" << endl;
}

int main() {
    int num_thread = 50;
    vector<thread> vecs;
    for (int i=0; i<num_thread; ++i) {
        if (i % 2 == 0)
            vecs.push_back(std::move(thread(put_to_q)));
        else 
            vecs.push_back(std::move(thread(take_to_q)));
    }
    for (auto &t : vecs) {
        t.join();
    }

    assert(q.empty());
    test_when_wait_destory();
    return 0;
}