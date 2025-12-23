# Система тестирования и покрытия кода

Этот проект настроен для автоматического тестирования, бенчмаркинга и анализа покрытия кода с использованием **Google Test**, **Boost Test Library** и **Google Benchmark**.

## Требования

- CMake 3.15 или выше
- Компилятор C++ с поддержкой C++17
- lcov (для генерации HTML отчетов о покрытии)
  - macOS: `brew install lcov`
  - Ubuntu/Debian: `sudo apt-get install lcov`
- Boost Test Library (опционально, для Boost тестов)
  - macOS: `brew install boost`
  - Ubuntu/Debian: `sudo apt-get install libboost-test-dev`

## Быстрый старт

### 1. Запуск всех тестов (Google Test + Boost Test)

```bash
./run_tests.sh
```

Это скомпилирует проект и запустит все тесты через Google Test и Boost Test (если установлен).

### 2. Запуск бенчмарков

```bash
./run_benchmarks.sh
```

Это скомпилирует проект и запустит все бенчмарки через Google Benchmark.

### 3. Генерация HTML отчета о покрытии кода

```bash
./run_coverage.sh
```

Этот скрипт:
1. Компилирует проект с флагами покрытия кода (`--coverage`)
2. Запускает все тесты (Google Test и Boost Test)
3. Собирает данные покрытия через lcov
4. Генерирует HTML отчет в директории `coverage_report/`
5. Выводит статистику покрытия в консоль

Откройте `coverage_report/index.html` в браузере для просмотра детального анализа покрытия кода.

## Ручная сборка

### Обычная сборка

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Сборка с покрытием кода

```bash
mkdir build_coverage
cd build_coverage
cmake .. -DENABLE_COVERAGE=ON
cmake --build .
./test_runner
./boost_test_runner  # если Boost установлен
```

## Структура проекта

- `include/` - Заголовочные файлы с реализацией структур данных
- `src/` - Исходный код основной программы
- `tests/` - Тесты и бенчмарки
  - `googletest.cpp` - Тесты через Google Test
  - `boosttest.cpp` - Тесты через Boost Test Library
  - `benchmark.cpp` - Бенчмарки через Google Benchmark
- `build/` - Директория для обычной сборки
- `build_coverage/` - Директория для сборки с покрытием
- `coverage_report/` - HTML отчеты о покрытии кода

## Используемые библиотеки

- **Google Test** - Фреймворк для unit-тестирования
- **Boost Test Library** - Альтернативный фреймворк для unit-тестирования
- **Google Benchmark** - Фреймворк для бенчмарков
- **lcov/genhtml** - Инструменты для анализа покрытия кода

Библиотеки Google Test и Google Benchmark автоматически загружаются через CMake FetchContent, если они не установлены в системе.

## Цель покрытия кода

Проект нацелен на достижение **не менее 85% покрытия кода** тестами. Тесты написаны с использованием как Google Test, так и Boost Test Library для обеспечения максимального покрытия.

## Просмотр отчета о покрытии

После выполнения `./run_coverage.sh`, откройте файл:
```
coverage_report/index.html
```

Отчет содержит:
- Общую статистику покрытия
- Покрытие по файлам
- Строки кода с указанием, какие были протестированы
- Визуальное представление покрытия (зеленый = покрыто, красный = не покрыто)

## Запуск отдельных тестов

### Только Google Test:
```bash
cd build
./test_runner
```

### Только Boost Test (если установлен):
```bash
cd build
./boost_test_runner
```

### Через CTest:
```bash
cd build
ctest -R GoogleTests    # только Google тесты
ctest -R BoostTests     # только Boost тесты
ctest                   # все тесты
```
