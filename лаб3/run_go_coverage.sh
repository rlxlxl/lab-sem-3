#!/bin/bash

# Скрипт для генерации HTML-отчета о покрытии кода для Go-проекта

set -e

# Проверка наличия Go
if ! command -v go &> /dev/null; then
    echo "❌ Go не установлен!"
    echo ""
    echo "📦 Для установки Go выполните:"
    echo "   ./install_go.sh"
    echo ""
    echo "Или вручную:"
    echo "   brew install go"
    exit 1
fi

echo "✅ Go установлен: $(go version)"
echo "🔍 Запуск тестов с покрытием кода..."

# Создаем директорию для отчетов
COVERAGE_DIR="go_coverage_report"
mkdir -p "$COVERAGE_DIR"

# Запускаем тесты с покрытием
go test -v -coverprofile="$COVERAGE_DIR/coverage.out" ./...

echo "📊 Генерация HTML-отчета..."

# Генерируем HTML-отчет
go tool cover -html="$COVERAGE_DIR/coverage.out" -o "$COVERAGE_DIR/coverage.html"

# Выводим статистику покрытия
echo ""
echo "📈 Статистика покрытия:"
go tool cover -func="$COVERAGE_DIR/coverage.out"

echo ""
echo "✅ HTML-отчет создан: $COVERAGE_DIR/coverage.html"
echo "🌐 Откройте файл в браузере для просмотра визуального анализа покрытия"

