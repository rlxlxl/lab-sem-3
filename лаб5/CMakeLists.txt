cmake_minimum_required(VERSION 3.16)
project(GameCharacterFactory)

# Используем C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Включаем заголовочные файлы
include_directories(include)

# Все исходники
set(SOURCES
    src/main.cpp
    src/Warrior.cpp
    src/Mage.cpp
    src/Archer.cpp
    src/EarthCharFactory.cpp
    src/WaterCharFactory.cpp
)

# Создаем исполняемый файл
add_executable(GameCharacterFactory ${SOURCES})
