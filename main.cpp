#include "SimpleThreadPool.h"
#include <iostream>
#include <mutex>

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

long long heavyTaskRec(int number)
{
    if (number == 0)
        return 1;
    return number * heavyTaskRec(number - 1);
}

long long heavyTask(ThreadSafeConsole& console, int number)
{
    console << heavyTaskRec(number) << std::endl; //race condition in between 2 operator<<
}

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
    ThreadSafeConsole console;
    SimpleThreadPool pool;
    pool.submitTask(heavyTask, std::ref(console), 20);
    pool.submitTask(heavyTask, std::ref(console), 20);
    pool.submitTask(heavyTask, std::ref(console), 20);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    return 0;
}

