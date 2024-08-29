#include "ThreadPool.hpp"
#include <functional>
#include <mutex>

ThreadPool::ThreadPool(const std::size_t workerCount) : stop(false) {
    for (size_t i = 0; i < workerCount; i++) {
        this->threads.emplace_back([this]() {
            while (true) {
                std::function<void()> task;
                {
                    std::unique_lock lock(this->queueLock);
                    this->cv.wait(lock, [this] {
                        return this->stop || !this->queue.empty();
                    });

                    if (this->stop && this->queue.empty()) {
                        return;
                    }

                    task = std::move(this->queue.front());
                    this->queue.pop();
                }

                task();
            }
        });
    }
}

ThreadPool::~ThreadPool() {
    std::unique_lock<std::mutex> lock(this->queueLock);
    this->stop = true;

    this->cv.notify_all();

    for (auto &thread : this->threads) {
        thread.join();
    }
}

void ThreadPool::enqueue(std::function<void()> function) {
    {
        std::unique_lock<std::mutex> lock(this->queueLock);
        this->queue.emplace(std::move(function));
    }

    this->cv.notify_one();
}
