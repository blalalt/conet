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
using namespace conet;
using namespace std;

ConcurrentQueue<int> q;

void put_to_q() {
    q.put(1);
}

void take_to_q() {
    int a ;
    q.take(a);
    // cout << a << endl;
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

    return 0;
}