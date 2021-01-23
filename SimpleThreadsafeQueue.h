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


template<typename T>
void SimpleThreadSafeQueue<T>::push(T value)
{
    std::lock_guard<std::mutex> lk(m);
    m_queue.push(std::move(value));
    m_condition.notify_one();
}

template<typename T>
bool SimpleThreadSafeQueue<T>::empty() const
{
    std::lock_guard<std::mutex> lk(m);
    return m_queue.empty();
}

template<typename T>
bool SimpleThreadSafeQueue<T>::tryPop(T& value)
{
    std::lock_guard<std::mutex> lk(m);

    if (m_queue.empty())
        return false;

    value = std::move(m_queue.front());
    m_queue.pop();
    return true;
}

template<typename T>
void SimpleThreadSafeQueue<T>::waitPop(T& value)
{
    std::unique_lock<std::mutex> ulk(m);
    m_condition.wait(ulk, [this](){return !m_queue.empty();});
    value = std::move(m_queue.front());
    m_queue.pop();
}
