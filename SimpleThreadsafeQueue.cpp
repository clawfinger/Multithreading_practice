#include "SimpleThreadsafeQueue.h"

template<typename T>
void ThreadSafeQueue<T>::push(T value)
{
    std::lock_guard<std::mutex> lk(m);
    m_queue.push(value);
    m_condition.notify_one();
}

template<typename T>
bool ThreadSafeQueue<T>::empty() const
{
    std::lock_guard<std::mutex> lk(m);
    return m_queue.empty();
}

template<typename T>
bool ThreadSafeQueue<T>::tryPop(T& value)
{
    std::lock_guard<std::mutex> lk(m);

    if (m_queue.empty())
        return false;

    value = m_queue.front();
    m_queue.pop();
    return true;
}

template<typename T>
void ThreadSafeQueue<T>::waitPop(T& value)
{
    std::unique_lock<std::mutex> ulk(m);
    m_condition.wait(ulk, [this](){return !m_queue.empty();});
    value = m_queue.front();
    m_queue.pop();
}
