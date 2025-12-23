#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>


class ReadersWriters {
private:
    mutable std::mutex mtx;
    std::condition_variable read_cv;
    std::condition_variable write_cv;
    
    int active_readers = 0;
    int waiting_readers = 0;
    int active_writers = 0;
    int waiting_writers = 0;
    
    bool reader_priority = true; // true - приоритет читателей, false - приоритет писателей
    
public:
    ReadersWriters(bool reader_priority = true) : reader_priority(reader_priority) {}
    
    void setPriority(bool reader_priority) {
        std::lock_guard<std::mutex> lock(mtx);
        this->reader_priority = reader_priority;
    }
    
    void startRead() {
        std::unique_lock<std::mutex> lock(mtx);
        waiting_readers++;
        
        // Если приоритет писателей и есть ожидающие писатели, ждем
        // Если приоритет читателей, читатели могут читать параллельно
        while (active_writers > 0 || 
               (!reader_priority && waiting_writers > 0 && active_readers == 0)) {
            read_cv.wait(lock);
        }
        
        waiting_readers--;
        active_readers++;
    }
    
    void endRead() {
        std::unique_lock<std::mutex> lock(mtx);
        active_readers--;
        
        if (active_readers == 0) {
            // Если есть ожидающие писатели, разбудим одного
            if (waiting_writers > 0) {
                write_cv.notify_one();
            }
            // Если приоритет читателей и есть ожидающие читатели, разбудим всех
            else if (reader_priority && waiting_readers > 0) {
                read_cv.notify_all();
            }
        }
    }
    
    void startWrite() {
        std::unique_lock<std::mutex> lock(mtx);
        waiting_writers++;
        
        // Писатель ждет, пока нет активных читателей и писателей
        // Если приоритет читателей, ждем также пока нет ожидающих читателей
        while (active_readers > 0 || active_writers > 0 ||
               (reader_priority && waiting_readers > 0)) {
            write_cv.wait(lock);
        }
        
        waiting_writers--;
        active_writers++;
    }
    
    void endWrite() {
        std::unique_lock<std::mutex> lock(mtx);
        active_writers--;
        
        // Если приоритет читателей, сначала разбудим читателей
        if (reader_priority) {
            if (waiting_readers > 0) {
                read_cv.notify_all();
            } else if (waiting_writers > 0) {
                write_cv.notify_one();
            }
        } else {
            // Если приоритет писателей, сначала разбудим писателя
            if (waiting_writers > 0) {
                write_cv.notify_one();
            } else if (waiting_readers > 0) {
                read_cv.notify_all();
            }
        }
    }
    
    int getActiveReaders() const {
        std::lock_guard<std::mutex> lock(mtx);
        return active_readers;
    }
    
    int getActiveWriters() const {
        std::lock_guard<std::mutex> lock(mtx);
        return active_writers;
    }
};

// Глобальные переменные
ReadersWriters rw(true); // По умолчанию приоритет читателей
int shared_data = 0;
int read_count = 0;
int write_count = 0;
std::mutex stats_mtx;

// Функция читателя
void reader(int id, int iterations) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(100, 500);
    
    for (int i = 0; i < iterations; ++i) {
        rw.startRead();
        
        // Чтение данных
        int value = shared_data;
        std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
        
        {
            std::lock_guard<std::mutex> lock(stats_mtx);
            read_count++;
        }
        
        std::cout << "Читатель " << id << " прочитал значение: " << value 
                  << " (активных читателей: " << rw.getActiveReaders() << ")" << std::endl;
        
        rw.endRead();
        
        // Пауза между операциями
        std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
    }
}

// Функция писателя
void writer(int id, int iterations) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(200, 800);
    
    for (int i = 0; i < iterations; ++i) {
        rw.startWrite();
        
        // Запись данных
        shared_data++;
        std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
        
        {
            std::lock_guard<std::mutex> lock(stats_mtx);
            write_count++;
        }
        
        std::cout << "Писатель " << id << " записал значение: " << shared_data 
                  << " (активных писателей: " << rw.getActiveWriters() << ")" << std::endl;
        
        rw.endWrite();
        
        // Пауза между операциями
        std::this_thread::sleep_for(std::chrono::milliseconds(dis(gen)));
    }
}

int main() {
    std::cout << "=== Задача читатели-писатели с приоритетами ===" << std::endl;
    std::cout << "Выберите режим приоритета:" << std::endl;
    std::cout << "1 - Приоритет читателей" << std::endl;
    std::cout << "2 - Приоритет писателей" << std::endl;
    std::cout << "Введите номер (1 или 2): ";
    
    int choice;
    std::cin >> choice;
    
    bool reader_priority = (choice == 1);
    rw.setPriority(reader_priority);
    
    std::cout << "\nВыбран режим: " << (reader_priority ? "Приоритет читателей" : "Приоритет писателей") << std::endl;
    std::cout << "Количество читателей: 5" << std::endl;
    std::cout << "Количество писателей: 3" << std::endl;
    std::cout << "Итераций на поток: 5" << std::endl;
    std::cout << std::endl;
    
    const int num_readers = 5;
    const int num_writers = 3;
    const int iterations = 5;
    
    std::vector<std::thread> threads;
    
    auto start = std::chrono::high_resolution_clock::now();
    
    // Запуск читателей
    for (int i = 0; i < num_readers; ++i) {
        threads.emplace_back(reader, i + 1, iterations);
    }
    
    // Запуск писателей
    for (int i = 0; i < num_writers; ++i) {
        threads.emplace_back(writer, i + 1, iterations);
    }
    
    // Ожидание завершения всех потоков
    for (auto& t : threads) {
        t.join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    std::cout << "\n=== Статистика ===" << std::endl;
    std::cout << "Всего операций чтения: " << read_count << std::endl;
    std::cout << "Всего операций записи: " << write_count << std::endl;
    std::cout << "Финальное значение shared_data: " << shared_data << std::endl;
    std::cout << "Время выполнения: " << duration.count() << " мс" << std::endl;
    
    return 0;
}

