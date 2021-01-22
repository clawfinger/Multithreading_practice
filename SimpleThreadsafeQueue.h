#pragma once
#include <mutex>
#include <queue>
#include <condition_variable>

template <typename T>
class SimpleThreadSafeQueue
{
public:
    SimpleThreadSafeQueue() = default;
    SimpleThreadSafeQueue(const SimpleThreadSafeQueue& other) = delete;
    SimpleThreadSafeQueue& operator=(const SimpleThreadSafeQueue& other) = delete;
    void push(T value);
    bool empty() const;

    bool tryPop(T& value);
    void waitPop(T& value);

private:
    mutable std::mutex m;
    std::queue<T> m_queue;
    std::condition_variable m_condition;
};
