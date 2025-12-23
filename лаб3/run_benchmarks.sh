#!/bin/bash

set -e

# Цвета для вывода
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== Запуск бенчмарков ===${NC}"

BUILD_DIR="build"

# Создание директории для сборки
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

echo -e "${YELLOW}Настройка CMake...${NC}"
cmake ..

echo -e "${YELLOW}Компиляция проекта...${NC}"
cmake --build . -- -j$(sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4)

echo -e "${YELLOW}Запуск бенчмарков...${NC}"
echo -e "${YELLOW}Это может занять некоторое время...${NC}"
./benchmark_runner

echo -e "\n${GREEN}=== Бенчмарки завершены! ===${NC}"


