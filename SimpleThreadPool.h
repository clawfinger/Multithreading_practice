#pragma once
#include <thread>
#include <vector>
#include <future>
#include <functional>
#include <type_traits>

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

class MovableCallableWrapper
{
    struct impl_base {
        virtual void call()=0;
        virtual ~impl_base() {}
    };

    template<typename F>
    struct impl_type: impl_base
    {
        F f;
        impl_type(F&& f_): f(std::move(f_)) {}
        void call() { f(); }
    };

    std::unique_ptr<impl_base> impl;

public:
    template<typename F>
    MovableCallableWrapper(F&& f):
        impl(new impl_type<F>(std::move(f))) {}
    void operator()() { impl->call(); }

    MovableCallableWrapper() = default;
    MovableCallableWrapper(MovableCallableWrapper&& other):
        impl(std::move(other.impl)){}

    MovableCallableWrapper& operator=(MovableCallableWrapper&& other)
    {
        impl=std::move(other.impl);
        return *this;
    }
    MovableCallableWrapper(const MovableCallableWrapper&)=delete;
    MovableCallableWrapper(MovableCallableWrapper&)=delete;
    MovableCallableWrapper& operator=(const MovableCallableWrapper&)=delete;
};

class SimpleThreadPool
{
public:
    SimpleThreadPool(unsigned int threadCount = std::thread::hardware_concurrency());

    template<typename Callable, typename... Args>
    decltype(auto) submitTask(Callable&& func, Args&&... args)
    {
        typedef typename std::result_of<Callable(Args...)>::type returnType;
        auto boundFunc = std::bind(std::forward<Callable>(func), std::forward<Args...>(args...));
        std::packaged_task<returnType()> task(boundFunc);
        std::future<returnType> future = task.get_future();
        m_queue.push(std::move(task));
        return future;
    }

private:
    void threadWorker();

private:
    SimpleThreadSafeQueue<MovableCallableWrapper> m_queue;
    std::vector<std::thread> m_threads;
    ThreadPoolJoinGuard m_joinGuard; //Member order is important! joinGuard should be destroyed before threads
};
