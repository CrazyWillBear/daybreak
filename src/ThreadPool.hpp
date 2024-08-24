#pragma once

#include <cstddef>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
private:
    std::vector<std::thread> threads;
    std::queue<std::function<void()>> queue;
    std::mutex queueLock;
    std::condition_variable cv;

    bool stop;

public:
    explicit ThreadPool(std::size_t workerCount);
    ~ThreadPool();

    void enqueue(std::function<void()> function);
};
