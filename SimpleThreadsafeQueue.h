#pragma once
#include <mutex>
#include <queue>
#include <condition_variable>

template <typename T>
class ThreadSafeQueue
{
public:
    ThreadSafeQueue() = default;
    ThreadSafeQueue(const ThreadSafeQueue& other) = delete;
    ThreadSafeQueue& operator=(const ThreadSafeQueue& other) = delete;
    void push(T value);
    bool empty() const;

    bool tryPop(T& value);
    void waitPop(T& value);

private:
    mutable std::mutex m;
    std::queue<T> m_queue;
    std::condition_variable m_condition;
};
