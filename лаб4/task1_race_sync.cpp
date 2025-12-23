#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <semaphore>
#include <barrier>
#include <atomic>
#include <chrono>
#include <random>
#include <algorithm>
#include <iomanip>

// StopWatch для измерения времени
class StopWatch {
private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;
    bool running = false;

public:
    void start() {
        start_time = std::chrono::high_resolution_clock::now();
        running = true;
    }

    void stop() {
        if (running) {
            end_time = std::chrono::high_resolution_clock::now();
            running = false;
        }
    }

    double getElapsedMilliseconds() const {
        if (running) {
            auto now = std::chrono::high_resolution_clock::now();
            return std::chrono::duration<double, std::milli>(now - start_time).count();
        }
        return std::chrono::duration<double, std::milli>(end_time - start_time).count();
    }
};

// SpinLock реализация
class SpinLock {
private:
    std::atomic<bool> locked{false};

public:
    void lock() {
        bool expected = false;
        while (!locked.compare_exchange_weak(expected, true, std::memory_order_acquire)) {
            expected = false;
        }
    }

    void unlock() {
        locked.store(false, std::memory_order_release);
    }
};

// SpinWait реализация (более эффективная версия SpinLock)
class SpinWait {
private:
    std::atomic<bool> locked{false};
    static constexpr int SPIN_COUNT = 1000;

public:
    void lock() {
        int spin_count = 0;
        bool expected = false;
        while (!locked.compare_exchange_weak(expected, true, std::memory_order_acquire)) {
            expected = false;
            if (++spin_count < SPIN_COUNT) {
                // CPU spin
                std::this_thread::yield();
            } else {
                // После SPIN_COUNT попыток, переходим к более агрессивному ожиданию
                std::this_thread::sleep_for(std::chrono::microseconds(1));
                spin_count = 0;
            }
        }
    }

    void unlock() {
        locked.store(false, std::memory_order_release);
    }
};

// Monitor реализация (обертка над mutex)
class Monitor {
private:
    std::mutex mtx;

public:
    void lock() {
        mtx.lock();
    }

    void unlock() {
        mtx.unlock();
    }

    std::unique_lock<std::mutex> getLock() {
        return std::unique_lock<std::mutex>(mtx);
    }
};

// Глобальные переменные для гонки
const int NUM_THREADS = 8;
const int ITERATIONS = 10000;
std::vector<char> shared_buffer;
std::vector<std::string> thread_results;

// Функция для генерации случайного ASCII символа
char getRandomASCII() {
    static thread_local std::random_device rd;
    static thread_local std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(33, 126); // Печатные ASCII символы
    return static_cast<char>(dis(gen));
}

// Тест с Mutex
void testMutex(int thread_id) {
    static std::mutex mtx;
    for (int i = 0; i < ITERATIONS; ++i) {
        std::lock_guard<std::mutex> lock(mtx);
        char c = getRandomASCII();
        shared_buffer.push_back(c);
        thread_results[thread_id] += c;
    }
}

// Тест с Semaphore
void testSemaphore(int thread_id) {
    static std::counting_semaphore<1> sem(1);
    for (int i = 0; i < ITERATIONS; ++i) {
        sem.acquire();
        char c = getRandomASCII();
        shared_buffer.push_back(c);
        thread_results[thread_id] += c;
        sem.release();
    }
}

// Тест с Barrier
void testBarrier(int thread_id) {
    static std::barrier<> bar(NUM_THREADS);
    for (int i = 0; i < ITERATIONS; ++i) {
        char c = getRandomASCII();
        thread_results[thread_id] += c;
        bar.arrive_and_wait();
        // После барьера все потоки синхронизированы
        if (thread_id == 0) {
            shared_buffer.push_back(c);
        }
    }
}

// Тест с SpinLock
void testSpinLock(int thread_id) {
    static SpinLock spinlock;
    for (int i = 0; i < ITERATIONS; ++i) {
        spinlock.lock();
        char c = getRandomASCII();
        shared_buffer.push_back(c);
        thread_results[thread_id] += c;
        spinlock.unlock();
    }
}

// Тест с SpinWait
void testSpinWait(int thread_id) {
    static SpinWait spinwait;
    for (int i = 0; i < ITERATIONS; ++i) {
        spinwait.lock();
        char c = getRandomASCII();
        shared_buffer.push_back(c);
        thread_results[thread_id] += c;
        spinwait.unlock();
    }
}

// Тест с Monitor
void testMonitor(int thread_id) {
    static Monitor monitor;
    for (int i = 0; i < ITERATIONS; ++i) {
        auto lock = monitor.getLock();
        char c = getRandomASCII();
        shared_buffer.push_back(c);
        thread_results[thread_id] += c;
    }
}

// Функция для запуска теста
double runTest(const std::string& test_name, void (*test_func)(int)) {
    shared_buffer.clear();
    thread_results.assign(NUM_THREADS, "");
    
    StopWatch sw;
    sw.start();
    
    std::vector<std::thread> threads;
    for (int i = 0; i < NUM_THREADS; ++i) {
        threads.emplace_back(test_func, i);
    }
    
    for (auto& t : threads) {
        t.join();
    }
    
    sw.stop();
    double elapsed = sw.getElapsedMilliseconds();
    
    std::cout << std::setw(15) << test_name << ": " 
              << std::fixed << std::setprecision(2) << elapsed << " ms"
              << " (Buffer size: " << shared_buffer.size() << ")" << std::endl;
    
    return elapsed;
}

int main() {
    std::cout << "=== Анализ примитивов синхронизации ===" << std::endl;
    std::cout << "Количество потоков: " << NUM_THREADS << std::endl;
    std::cout << "Итераций на поток: " << ITERATIONS << std::endl;
    std::cout << "Всего операций: " << NUM_THREADS * ITERATIONS << std::endl;
    std::cout << std::endl;
    
    std::vector<std::pair<std::string, double>> results;
    
    // Запуск всех тестов
    results.push_back({"Mutex", runTest("Mutex", testMutex)});
    results.push_back({"Semaphore", runTest("Semaphore", testSemaphore)});
    results.push_back({"Barrier", runTest("Barrier", testBarrier)});
    results.push_back({"SpinLock", runTest("SpinLock", testSpinLock)});
    results.push_back({"SpinWait", runTest("SpinWait", testSpinWait)});
    results.push_back({"Monitor", runTest("Monitor", testMonitor)});
    
    // Сортировка результатов по времени
    std::sort(results.begin(), results.end(), 
              [](const auto& a, const auto& b) { return a.second < b.second; });
    
    std::cout << std::endl << "=== Рейтинг по скорости (от быстрого к медленному) ===" << std::endl;
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << (i + 1) << ". " << std::setw(15) << results[i].first 
                  << ": " << std::fixed << std::setprecision(2) << results[i].second << " ms" << std::endl;
    }
    
    // Сравнительный анализ
    if (results.size() > 1) {
        double fastest = results[0].second;
        std::cout << std::endl << "=== Сравнительный анализ ===" << std::endl;
        for (const auto& r : results) {
            double ratio = r.second / fastest;
            std::cout << r.first << " медленнее в " << std::fixed << std::setprecision(2) 
                      << ratio << " раз(а)" << std::endl;
        }
    }
    
    return 0;
}

