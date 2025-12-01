cmake_minimum_required(VERSION 3.10)

project(CodeToHtml LANGUAGES CXX)

# Используем C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Пути к заголовкам
include_directories(include)

# Собираем файлы
file(GLOB SOURCES
        src/*.cpp
)

# Создаем исполняемый файл
add_executable(CodeToHtml ${SOURCES})

# Флаги компиляции
if (MSVC)
    target_compile_options(CodeToHtml PRIVATE /W4)
else()
    target_compile_options(CodeToHtml PRIVATE -Wall -Wextra -pedantic)
endif()
