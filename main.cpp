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



int main()
{
    ThreadSafeConsole console;
    while(true)
    {
        int num = 0;
        console >> num;
        console << heavyTask(num) << std::endl;
    }
    return 0;
}

