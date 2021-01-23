#include "SimpleThreadPool.h"
#include <iostream>
#include <mutex>

long long heavyTask(int number)
{
    if (number == 0)
        return 1;
    return number * heavyTask(number - 1);
}

class ThreadSafeConsole
{
public:
    ThreadSafeConsole() = default;

    template<typename T>
    void operator>>(T& num)
    {
        std::lock_guard<std::mutex> lk(m);
        std::cin >> num;
    }

    template<typename T>
    std::ostream& operator<<(T num)
    {
        std::lock_guard<std::mutex> lk(m);
        std::cout << num;
        return std::cout;
    }

private:
    std::mutex m;
};

template <class callable, class... arguments>
void later(int after, callable&& f, arguments&&... args)
{
    std::function<typename std::result_of<callable(arguments...)>::type()> task(std::bind(std::forward<callable>(f), std::forward<arguments>(args)...));

    std::thread([after, task]() {
        std::this_thread::sleep_for(std::chrono::milliseconds(after));
        task();
    }).detach();
}

int main()
{
//    std::queue<std::packaged_task<void()>> queue;
//    auto boundFunc = std::bind(heavyTask, 10);
//    std::packaged_task<long long()> task(boundFunc);
//    std::future<long long> future = task.get_future();
//    queue.push(std::packaged_task<void()>(std::move(task)));
//    std::packaged_task<void()> task1 = std::move(queue.front());

    SimpleThreadPool pool;
    std::future<long long> future = pool.submitTask(heavyTask, 4);
    long long res = future.get();
    return 0;
}

