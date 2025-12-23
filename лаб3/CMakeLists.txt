cmake_minimum_required(VERSION 3.15)
project(Lab3 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Настройка для покрытия кода
option(ENABLE_COVERAGE "Enable code coverage" OFF)
if(ENABLE_COVERAGE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} --coverage -fprofile-arcs -ftest-coverage -O0 -g")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} --coverage -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --coverage")
    message(STATUS "Code coverage enabled")
endif()

# Включаемые директории
include_directories(include)

# Тесты с Google Test
enable_testing()
find_package(GTest QUIET)
if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
    )
    FetchContent_MakeAvailable(googletest)
endif()

# Основная программа
add_executable(prog src/main.cpp)
target_link_libraries(prog GTest::gtest GTest::gtest_main)

add_executable(test_runner tests/googletest.cpp)
target_link_libraries(test_runner GTest::gtest GTest::gtest_main)

if(ENABLE_COVERAGE)
    target_compile_options(test_runner PRIVATE --coverage -fprofile-arcs -ftest-coverage)
    target_link_options(test_runner PRIVATE --coverage)
endif()

add_test(NAME GoogleTests COMMAND test_runner)

# Тесты с Boost Test
find_package(Boost QUIET COMPONENTS unit_test_framework)
if(Boost_FOUND)
    add_executable(boost_test_runner tests/boosttest.cpp)
    target_link_libraries(boost_test_runner Boost::unit_test_framework)
    target_include_directories(boost_test_runner PRIVATE include)
    
    if(ENABLE_COVERAGE)
        target_compile_options(boost_test_runner PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_link_options(boost_test_runner PRIVATE --coverage)
    endif()
    
    add_test(NAME BoostTests COMMAND boost_test_runner)
    message(STATUS "Boost Test enabled")
else()
    message(WARNING "Boost Test Library not found. Install Boost to enable Boost tests.")
    message(STATUS "  macOS: brew install boost")
    message(STATUS "  Ubuntu/Debian: sudo apt-get install libboost-test-dev")
endif()

# Бенчмарки с Google Benchmark
find_package(benchmark QUIET)
if(NOT benchmark_FOUND)
    include(FetchContent)
    FetchContent_Declare(
        googlebenchmark
        GIT_REPOSITORY https://github.com/google/benchmark.git
        GIT_TAG v1.8.3
    )
    set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "")
    FetchContent_MakeAvailable(googlebenchmark)
endif()

add_executable(benchmark_runner tests/benchmark.cpp)
target_link_libraries(benchmark_runner benchmark::benchmark benchmark::benchmark_main)

# Добавляем зависимости на библиотеки для правильной сборки
target_include_directories(test_runner PRIVATE include)
target_include_directories(benchmark_runner PRIVATE include)
