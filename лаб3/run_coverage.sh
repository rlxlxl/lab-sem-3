#!/bin/bash

set -e

# Цвета для вывода
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}=== Настройка системы тестирования и покрытия кода ===${NC}"

# Проверка наличия необходимых инструментов
echo -e "${YELLOW}Проверка инструментов...${NC}"

if ! command -v lcov &> /dev/null; then
    echo -e "${RED}Ошибка: lcov не установлен${NC}"
    echo "Установите lcov:"
    echo "  macOS: brew install lcov"
    echo "  Ubuntu/Debian: sudo apt-get install lcov"
    exit 1
fi

if ! command -v genhtml &> /dev/null; then
    echo -e "${RED}Ошибка: genhtml не установлен (часть lcov)${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Все инструменты найдены${NC}"

# Создание директорий
BUILD_DIR="build_coverage"
REPORT_DIR="coverage_report"

echo -e "${YELLOW}Очистка предыдущих сборок...${NC}"
rm -rf ${BUILD_DIR} ${REPORT_DIR}

echo -e "${YELLOW}Создание директории для сборки с покрытием...${NC}"
mkdir -p ${BUILD_DIR}
cd ${BUILD_DIR}

echo -e "${YELLOW}Настройка CMake с включенным покрытием кода...${NC}"
cmake .. -DENABLE_COVERAGE=ON

echo -e "${YELLOW}Компиляция проекта...${NC}"
cmake --build . -- -j$(sysctl -n hw.ncpu 2>/dev/null || nproc 2>/dev/null || echo 4)

echo -e "${YELLOW}Запуск тестов...${NC}"
ctest --output-on-failure || true

echo -e "${YELLOW}Запуск test_runner для генерации данных покрытия...${NC}"
./test_runner

if [ -f "./boost_test_runner" ]; then
    echo -e "${YELLOW}Запуск boost_test_runner для генерации данных покрытия...${NC}"
    ./boost_test_runner
fi

echo -e "${YELLOW}Сборка данных покрытия кода...${NC}"

# Инициализация lcov - собираем данные из текущей директории
# Игнорируем ошибки несовместимости версий gcov
lcov --capture --directory . --output-file coverage.info \
    --ignore-errors inconsistent,unsupported 2>/dev/null || true

# Удаление системных заголовков и нерелевантных файлов
lcov --remove coverage.info \
    '/usr/*' \
    '/opt/*' \
    '*/googletest/*' \
    '*/benchmark/*' \
    '*/boost/*' \
    '*/tests/*' \
    '*/include/googletest/*' \
    '*/include/benchmark/*' \
    '*/CMakeFiles/*' \
    --output-file coverage.filtered.info \
    --ignore-errors inconsistent,unsupported 2>/dev/null || true

cd ..

echo -e "${YELLOW}Генерация HTML отчета...${NC}"
# Генерируем HTML отчет с игнорированием ошибок для шаблонных классов
# Используем дополнительные флаги для включения всех файлов
genhtml "${BUILD_DIR}/coverage.filtered.info" --output-directory "${REPORT_DIR}" \
    --title "Отчет о покрытии кода - Лабораторная работа 3" \
    --ignore-errors inconsistent,unsupported,missing \
    --show-details \
    --function-coverage \
    --branch-coverage \
    --legend \
    --num-spaces 4 \
    --precision 2 2>&1 | grep -v "WARNING\|ERROR" || true

# Проверяем, какие файлы попали в отчет
echo -e "${YELLOW}Проверка наличия всех файлов в отчете...${NC}"
mkdir -p "${REPORT_DIR}/include"
REPORTED_FILES=0
MISSING_FILES=""

for file in array doublylist forwardlist fullbinarytree hashtable queue stack; do
    if [ -f "${REPORT_DIR}/include/${file}.hpp.gcov.html" ]; then
        echo -e "${GREEN}  ✓ ${file}.hpp.gcov.html найден${NC}"
        REPORTED_FILES=$((REPORTED_FILES + 1))
    else
        echo -e "${YELLOW}  ✗ ${file}.hpp.gcov.html не найден${NC}"
        MISSING_FILES="${MISSING_FILES} ${file}"
    fi
done

# Если некоторые файлы не попали, пытаемся создать для них базовые отчеты
if [ ! -z "$MISSING_FILES" ]; then
    echo -e "${YELLOW}Попытка добавить недостающие файлы в отчет...${NC}"
    cd "${BUILD_DIR}"
    
    for file in $MISSING_FILES; do
        # Создаем отдельный файл coverage только для этого файла
        lcov --extract coverage.filtered.info "*${file}.hpp" --output-file "${file}_only.info" 2>/dev/null || true
        if [ -f "${file}_only.info" ] && [ -s "${file}_only.info" ]; then
            # Генерируем HTML только для этого файла
            genhtml "${file}_only.info" --output-directory "../${REPORT_DIR}" \
                --title "Отчет о покрытии кода - ${file}.hpp" \
                --ignore-errors inconsistent,unsupported,missing \
                --show-details --function-coverage 2>/dev/null || true
            rm -f "${file}_only.info"
        fi
    done
    cd ..
    
    # Повторная проверка
    echo -e "${YELLOW}Повторная проверка после добавления...${NC}"
    for file in $MISSING_FILES; do
        if [ -f "${REPORT_DIR}/include/${file}.hpp.gcov.html" ]; then
            echo -e "${GREEN}  ✓ ${file}.hpp.gcov.html теперь найден${NC}"
            REPORTED_FILES=$((REPORTED_FILES + 1))
        fi
    done
fi

echo -e "${GREEN}Создано HTML отчетов: ${REPORTED_FILES} из 7${NC}"

# Показ статистики покрытия
echo -e "\n${GREEN}=== Статистика покрытия кода ===${NC}"
lcov --summary ${BUILD_DIR}/coverage.filtered.info \
    --ignore-errors inconsistent,unsupported 2>/dev/null || true

echo -e "\n${GREEN}=== Готово! ===${NC}"
echo -e "HTML отчет доступен в: ${GREEN}${REPORT_DIR}/index.html${NC}"
echo -e "Откройте файл в браузере для просмотра детального анализа покрытия кода."

