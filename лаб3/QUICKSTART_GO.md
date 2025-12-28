# Быстрый старт - Go Coverage Report

## Проблема: "go: command not found"

Если вы видите эту ошибку, значит Go не установлен в системе.

## Решение

### Шаг 1: Установите Go

Выполните в терминале:
```bash
./install_go.sh
```

Или вручную:
```bash
brew install go
```

### Шаг 2: Установите зависимости проекта

```bash
go mod tidy
```

### Шаг 3: Сгенерируйте HTML-отчет

```bash
./run_go_coverage.sh
```

### Шаг 4: Откройте отчет в браузере

```bash
open go_coverage_report/coverage.html
```

## Что дальше?

После установки Go вы сможете:
- ✅ Запускать тесты: `go test -v ./...`
- ✅ Генерировать HTML-отчеты о покрытии
- ✅ Видеть визуальный анализ покрытия кода

## Структура проекта

- `calculator.go` - исходный код
- `calculator_test.go` - тесты с testify
- `run_go_coverage.sh` - скрипт генерации отчета
- `install_go.sh` - скрипт установки Go
- `go_coverage_report/` - директория с отчетами

## Дополнительная информация

Подробная документация: `README_GO_COVERAGE.md`

