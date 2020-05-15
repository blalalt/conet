/*
 * @Description: 
 * @version: 
 * @Author: blalalt
 * @Date: 2020-05-15 09:30:00
 */
#include "../utils/ThreadPool.h"
#include <unistd.h>
#include <iostream>
#include <functional>

// using namespace std;
using namespace conet;


void func(int i) {
    std::cout << i << std::endl;
    // sleep(1);
}

int main() {
    ThreadPool<std::function<void()>> pool(4);
    for (int i=0; i<4; i++) {
        pool.add_task(std::bind(func, i));
    }
    pool.start();

    for (int i=0; i<400; i++) {
        pool.add_task(std::bind(func, i));
    }
    // pool.stop();
    // sleep(10);
    return 0;
}