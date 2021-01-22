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
    //    for (unsigned int i = 0; i < threadCount; ++i)
}

void SimpleThreadPool::threadWorker()
{
    while(true) //TODO: add stop flag;
    {
        m_queue.waitPop()
    }
}
