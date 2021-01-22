#pragma once
#include <thread>
#include <vector>
#include <future>

#include "SimpleThreadsafeQueue.h"

class ThreadPoolJoinGuard
{
public:
    ThreadPoolJoinGuard(std::vector<std::thread>& poolThreads);
    ~ThreadPoolJoinGuard();
    ThreadPoolJoinGuard(const ThreadPoolJoinGuard& other) = delete;
    ThreadPoolJoinGuard& operator=(const ThreadPoolJoinGuard& other) = delete;

private:
    std::vector<std::thread>& m_poolThreads;
};

class SimpleThreadPool
{
public:
    SimpleThreadPool(unsigned int threadCount = std::thread::hardware_concurrency());

private:
    void threadWorker();

private:
    SimpleThreadSafeQueue<int> m_queue;
    std::vector<std::thread> m_threads;
    ThreadPoolJoinGuard m_joinGuard; //Member order is important! joinGuard should be destroyed before threads
};
