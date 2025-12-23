#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <algorithm>
#include <ctime>
#include <sstream>
#include <thread>
#include <mutex>
#include <chrono>
#include <map>

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

// Структура для хранения данных о тренировке
struct Training {
    std::string date;      // Дата в формате YYYY-MM-DD
    std::string time;      // Время в формате HH:MM
    std::string trainer_name; // ФИО тренера
    
    Training(const std::string& d, const std::string& t, const std::string& name)
        : date(d), time(t), trainer_name(name) {}
};

// Функция для получения дня недели из даты
// Возвращает: 0=воскресенье, 1=понедельник, ..., 6=суббота
int getDayOfWeek(const std::string& date) {
    std::tm tm = {};
    std::istringstream ss(date);
    ss >> std::get_time(&tm, "%Y-%m-%d");
    
    if (ss.fail()) {
        return -1; // Ошибка парсинга
    }
    
    std::time_t time = std::mktime(&tm);
    std::tm* local_tm = std::localtime(&time);
    
    return local_tm->tm_wday;
}

// Функция для получения названия дня недели
std::string getDayName(int dayOfWeek) {
    const std::vector<std::string> days = {
        "Воскресенье", "Понедельник", "Вторник", "Среда",
        "Четверг", "Пятница", "Суббота"
    };
    
    if (dayOfWeek >= 0 && dayOfWeek < 7) {
        return days[dayOfWeek];
    }
    return "Неизвестно";
}

// Функция для поиска тренировок в заданный день недели
std::vector<Training> findTrainingsByDay(const std::vector<Training>& trainings, int targetDay) {
    std::vector<Training> result;
    
    for (const auto& training : trainings) {
        int day = getDayOfWeek(training.date);
        if (day == targetDay) {
            result.push_back(training);
        }
    }
    
    return result;
}

// Функция для вывода тренировок
void printTrainings(const std::vector<Training>& trainings, const std::string& title) {
    std::cout << "\n=== " << title << " ===" << std::endl;
    
    if (trainings.empty()) {
        std::cout << "Тренировки не найдены." << std::endl;
        return;
    }
    
    std::cout << std::left << std::setw(15) << "Дата" 
              << std::setw(10) << "Время"
              << std::setw(30) << "ФИО тренера"
              << std::setw(15) << "День недели" << std::endl;
    std::cout << std::string(70, '-') << std::endl;
    
    for (const auto& training : trainings) {
        int day = getDayOfWeek(training.date);
        std::cout << std::left << std::setw(15) << training.date
                  << std::setw(10) << training.time
                  << std::setw(30) << training.trainer_name
                  << std::setw(15) << getDayName(day) << std::endl;
    }
}

// Однопоточная обработка: поиск тренировок по всем дням недели
std::map<int, std::vector<Training>> processTrainingsSingleThread(const std::vector<Training>& trainings) {
    std::map<int, std::vector<Training>> results;
    
    for (int day = 0; day < 7; ++day) {
        std::vector<Training> found = findTrainingsByDay(trainings, day);
        if (!found.empty()) {
            results[day] = found;
        }
    }
    
    return results;
}

// Многопоточная обработка: поиск тренировок по всем дням недели
std::map<int, std::vector<Training>> processTrainingsMultiThread(const std::vector<Training>& trainings) {
    std::map<int, std::vector<Training>> results;
    std::mutex results_mutex;
    std::vector<std::thread> threads;
    
    // Создаем поток для каждого дня недели
    for (int day = 0; day < 7; ++day) {
        threads.emplace_back([&trainings, &results, &results_mutex, day]() {
            std::vector<Training> found = findTrainingsByDay(trainings, day);
            if (!found.empty()) {
                std::lock_guard<std::mutex> lock(results_mutex);
                results[day] = found;
            }
        });
    }
    
    // Ждем завершения всех потоков
    for (auto& t : threads) {
        t.join();
    }
    
    return results;
}

// Функция для вывода результатов обработки
void printProcessingResults(const std::map<int, std::vector<Training>>& results) {
    std::cout << "\n=== Результаты обработки ===" << std::endl;
    int total_found = 0;
    
    for (const auto& pair : results) {
        int day = pair.first;
        const auto& trainings = pair.second;
        total_found += trainings.size();
        std::cout << getDayName(day) << ": найдено " << trainings.size() << " тренировок" << std::endl;
    }
    
    std::cout << "\nВсего найдено тренировок: " << total_found << std::endl;
}

int main() {
    // Создание тестовых данных о тренировках
    // Увеличиваем количество данных для более заметной разницы в производительности
    std::vector<Training> trainings = {
        Training("2024-01-15", "10:00", "Иванов Иван Иванович"),      // Понедельник
        Training("2024-01-16", "14:30", "Петрова Мария Сергеевна"),   // Вторник
        Training("2024-01-17", "18:00", "Сидоров Петр Александрович"), // Среда
        Training("2024-01-18", "09:00", "Козлова Анна Дмитриевна"),   // Четверг
        Training("2024-01-19", "16:00", "Морозов Дмитрий Викторович"), // Пятница
        Training("2024-01-20", "11:00", "Волкова Елена Николаевна"),  // Суббота
        Training("2024-01-21", "15:00", "Лебедев Сергей Олегович"),   // Воскресенье
        Training("2024-01-22", "10:00", "Иванов Иван Иванович"),      // Понедельник
        Training("2024-01-23", "14:30", "Петрова Мария Сергеевна"),   // Вторник
        Training("2024-01-24", "18:00", "Сидоров Петр Александрович"), // Среда
    };
    
    // Дублируем данные для более заметной разницы в производительности
    for (int i = 0; i < 5; ++i) {
        for (size_t j = 0; j < 10; ++j) {
            trainings.push_back(trainings[j]);
        }
    }
    
    std::cout << "=== Анализ производительности обработки тренировок ===" << std::endl;
    std::cout << "Всего тренировок для обработки: " << trainings.size() << std::endl;
    std::cout << std::endl;
    
    // ========== ОДНОПОТОЧНАЯ ОБРАБОТКА ==========
    std::cout << "=== Однопоточная обработка ===" << std::endl;
    StopWatch sw_single;
    sw_single.start();
    
    std::map<int, std::vector<Training>> results_single = processTrainingsSingleThread(trainings);
    
    sw_single.stop();
    double time_single = sw_single.getElapsedMilliseconds();
    
    std::cout << "Время обработки: " << std::fixed << std::setprecision(4) 
              << time_single << " мс" << std::endl;
    printProcessingResults(results_single);
    
    // ========== МНОГОПОТОЧНАЯ ОБРАБОТКА ==========
    std::cout << "\n\n=== Многопоточная обработка ===" << std::endl;
    StopWatch sw_multi;
    sw_multi.start();
    
    std::map<int, std::vector<Training>> results_multi = processTrainingsMultiThread(trainings);
    
    sw_multi.stop();
    double time_multi = sw_multi.getElapsedMilliseconds();
    
    std::cout << "Время обработки: " << std::fixed << std::setprecision(4) 
              << time_multi << " мс" << std::endl;
    printProcessingResults(results_multi);
    
    // ========== СРАВНЕНИЕ РЕЗУЛЬТАТОВ ==========
    std::cout << "\n\n=== Сравнение результатов ===" << std::endl;
    std::cout << std::fixed << std::setprecision(4);
    std::cout << "Однопоточная обработка: " << time_single << " мс" << std::endl;
    std::cout << "Многопоточная обработка: " << time_multi << " мс" << std::endl;
    
    if (time_single > 0) {
        double speedup = time_single / time_multi;
        std::cout << "Ускорение: " << std::setprecision(2) << speedup << "x" << std::endl;
        
        if (speedup > 1.0) {
            std::cout << "Многопоточная обработка быстрее на " 
                      << std::setprecision(1) << ((speedup - 1.0) * 100) << "%" << std::endl;
        } else if (speedup < 1.0) {
            std::cout << "Однопоточная обработка быстрее на " 
                      << std::setprecision(1) << ((1.0 / speedup - 1.0) * 100) << "%" << std::endl;
        } else {
            std::cout << "Производительность одинаковая" << std::endl;
        }
    }
    
    // Проверка корректности результатов
    bool results_match = true;
    if (results_single.size() != results_multi.size()) {
        results_match = false;
    } else {
        for (const auto& pair : results_single) {
            int day = pair.first;
            if (results_multi.find(day) == results_multi.end() ||
                results_multi[day].size() != pair.second.size()) {
                results_match = false;
                break;
            }
        }
    }
    
    std::cout << "\nКорректность результатов: " 
              << (results_match ? "✓ Результаты совпадают" : "✗ Результаты не совпадают") 
              << std::endl;
    
    // ========== ДЕТАЛЬНЫЙ ВЫВОД ТРЕНИРОВОК ==========
    std::cout << "\n\n=== Детальный вывод всех тренировок ===" << std::endl;
    printTrainings(trainings, "Все тренировки");
    
    // Интерактивный поиск
    std::cout << "\n\n=== Интерактивный поиск ===" << std::endl;
    std::cout << "Введите день недели (0=воскресенье, 1=понедельник, ..., 6=суббота): ";
    
    int targetDay;
    if (std::cin >> targetDay) {
        if (targetDay >= 0 && targetDay < 7) {
            std::vector<Training> found = findTrainingsByDay(trainings, targetDay);
            printTrainings(found, "Результаты поиска: " + getDayName(targetDay));
        } else {
            std::cout << "Неверный день недели. Должно быть от 0 до 6." << std::endl;
        }
    }
    
    return 0;
}

