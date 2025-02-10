#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <functional>

std::atomic<bool> running1(true);
std::atomic<bool> running2(true);
std::atomic<bool> running3(true);
std::atomic<bool> running4(true);
std::atomic<bool> running5(true);

void threadFunction1() {
    while (running1.load()) {
        std::cout << "Thread 1 is running.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

void threadFunction2() {
    while (running2.load()) {
        std::cout << "Thread 2 is running.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(600));
    }
}

void threadFunction3() {
    auto start_time = std::chrono::steady_clock::now();
    while (running3.load()) {
        std::cout << "Thread 3 is running.";
        std::this_thread::sleep_for(std::chrono::milliseconds(700));
        if (std::chrono::steady_clock::now() - start_time > std::chrono::seconds(10)) {
            int* ptr = nullptr;
            *ptr = 42; // Intentional segmentation fault (coredump)
        }
    }
    int* ptr = nullptr;
    *ptr = 42; // Intentional segmentation fault (coredump)
    while (running3.load()) {
        std::cout << "Thread 3 is running.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(700));
    }
}

void threadFunction4() {
    while (running4.load()) {
        std::cout << "Thread 4 is running.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(800));
    }
}

void threadFunction5(std::function<void()> callback) {
    while (running5.load()) {
        callback();
        std::this_thread::sleep_for(std::chrono::milliseconds(900));
    }
}

int main() {
    std::vector<std::thread> threads;

    threads.emplace_back(threadFunction1);
    threads.emplace_back(threadFunction2);
    threads.emplace_back(threadFunction3);
    threads.emplace_back(threadFunction4);
    
    auto callback = []() {
        std::cout << "Callback Thread 5 is executing.\n";
    };
    threads.emplace_back(threadFunction5, callback);

    for (auto &t : threads) {
        if (t.joinable()) {
            t.join();
        }
    }

    return 0;
}
