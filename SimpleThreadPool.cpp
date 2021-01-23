#include "SimpleThreadPool.h"

ThreadPoolJoinGuard::ThreadPoolJoinGuard(std::vector<std::thread>& poolThreads):
    m_poolThreads(poolThreads)
{

}

ThreadPoolJoinGuard::~ThreadPoolJoinGuard()
{
    for(auto& thread: m_poolThreads)
        thread.join();
}

SimpleThreadPool::SimpleThreadPool(unsigned int threadCount): m_joinGuard(m_threads)
{
    m_stop.store(false);
    for (unsigned int i = 0; i < threadCount; ++i)
        m_threads.push_back(std::thread(&SimpleThreadPool::threadWorker, this));
}

SimpleThreadPool::~SimpleThreadPool()
{
    m_stop.store(true);
}

void SimpleThreadPool::threadWorker()
{
    while(!m_stop) //TODO: add stop flag;
    {
        MovableCallableWrapper threadTask;
        if (m_queue.tryPop(threadTask))
            threadTask();
        else
            std::this_thread::yield();
    }
}
