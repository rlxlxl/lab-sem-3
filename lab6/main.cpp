#include <iostream>
#include <string>
#include <fstream>
#include <locale>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <sstream>
#include <pqxx/pqxx>

// Чтение SQL файла
std::string read_sql_file(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Не удалось открыть файл: " + filename);
    }
    
    std::string sql_content;
    std::string line;
    while (std::getline(file, line)) {
        sql_content += line + "\n";
    }
    file.close();
    return sql_content;
}

// Функция для получения строкового представления значения ячейки
std::string get_cell_value(const pqxx::field& field) {
    if (field.is_null()) {
        return "(NULL)";
    }
    std::string value = field.c_str();
    // Если пустая строка, показываем специальный маркер
    if (value.empty()) {
        return "(пусто)";
    }
    return value;
}

// Функция для вывода результатов списком
void print_table(const pqxx::result& res) {
    if (res.empty()) {
        std::cout << "Запрос выполнен успешно (нет возвращаемых строк)" << std::endl;
        return;
    }
    
    int row_num = 1;
    for (auto row : res) {
        std::cout << "\n--- Строка #" << row_num << " ---" << std::endl;
        for (pqxx::row::size_type i = 0; i < row.size(); ++i) {
            std::string column_name = res.column_name(i);
            std::string cell_value = get_cell_value(row[i]);
            std::cout << "  " << column_name << ": " << cell_value << std::endl;
        }
        row_num++;
    }
    
    std::cout << "\nВсего строк: " << res.size() << std::endl;
}

// Функция для чтения многострочного SQL-запроса
std::string read_multiline_query() {
    std::string query;
    std::string line;
    
    std::cout << "Введите SQL-запрос (для завершения введите пустую строку или ';'):" << std::endl;
    std::cout << "> ";
    
    while (std::getline(std::cin, line)) {
        // Если пустая строка и запрос уже начат, завершаем
        if (line.empty() && !query.empty()) {
            break;
        }
        // Если строка содержит только точку с запятой, завершаем
        if (line.find_first_not_of(" \t;") == std::string::npos && !query.empty()) {
            break;
        }
        
        if (!line.empty()) {
            query += line + " ";
        }
    }
    
    // Убираем лишние пробелы и добавляем точку с запятой если нужно
    if (!query.empty()) {
        // Убираем пробелы в начале и конце
        size_t start = query.find_first_not_of(" \t\n");
        size_t end = query.find_last_not_of(" \t\n");
        if (start != std::string::npos && end != std::string::npos) {
            query = query.substr(start, end - start + 1);
        }
        
        // Добавляем точку с запятой если её нет
        if (query.back() != ';') {
            query += ";";
        }
    }
    
    return query;
}

int main() {
    try {
        // Настройка локали для корректного вывода кириллицы
        std::locale::global(std::locale(""));
        std::cout.imbue(std::locale(""));
        std::cerr.imbue(std::locale(""));
        
        // Строка подключения
        std::string connection_str = 
            "dbname=lab6 user=postgres password=postgres host=localhost port=5432";
        
        std::cout << "Подключение к PostgreSQL..." << std::endl;
        pqxx::connection conn(connection_str);
        
        if (conn.is_open()) {
            std::cout << "✓ Подключено к базе данных: " << conn.dbname() << std::endl;
        } else {
            std::cerr << "✗ Ошибка подключения!" << std::endl;
            return 1;
        }
        
        // 1. Создание таблиц и заполнение данных
        std::cout << "\n=== Создание таблиц и вставка данных ===" << std::endl;
        {
            pqxx::work txn(conn);
            std::string schema_sql = read_sql_file("schema.sql");
            txn.exec(schema_sql);
            txn.commit();
            std::cout << "✓ Схема базы данных успешно создана!" << std::endl;
        }
        
        // 2. Интерактивный режим для ввода SQL-запросов
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "=== Интерактивный режим работы с базой данных ===" << std::endl;
        std::cout << "Введите SQL-запросы для выполнения." << std::endl;
        std::cout << "Для выхода введите: exit, quit или \\q" << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
        int query_count = 0;
        while (true) {
            std::cout << "\n";
            std::string query = read_multiline_query();
            
            // Проверка на команды выхода
            std::string query_lower = query;
            std::transform(query_lower.begin(), query_lower.end(), query_lower.begin(), ::tolower);
            if (query.empty() || 
                query_lower == "exit" || query_lower == "exit;" ||
                query_lower == "quit" || query_lower == "quit;" ||
                query_lower == "\\q" || query_lower == "\\q;") {
                break;
            }
            
            // Пропускаем комментарии
            if (query.find("--") == 0) {
                continue;
            }
            
            query_count++;
            std::cout << "\n--- Запрос #" << query_count << " ---" << std::endl;
            
            try {
                pqxx::work txn(conn);
                pqxx::result res = txn.exec(query);
                txn.commit();
                
                // Выводим результаты в виде красивой таблицы
                print_table(res);
                
            } catch (const std::exception& e) {
                std::cerr << "✗ Ошибка выполнения запроса: " << e.what() << std::endl;
            }
        }
        
        std::cout << "\n" << std::string(60, '=') << std::endl;
        std::cout << "=== Работа завершена ===" << std::endl;
        std::cout << "Выполнено запросов: " << query_count << std::endl;
        std::cout << std::string(60, '=') << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "\n✗ Ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}