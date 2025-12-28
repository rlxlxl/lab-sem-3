#!/bin/bash

# Скрипт для установки Go на macOS через Homebrew

set -e

echo "🔍 Проверка установки Go..."

if command -v go &> /dev/null; then
    echo "✅ Go уже установлен!"
    go version
    exit 0
fi

echo "📦 Установка Go через Homebrew..."
echo "Это может занять несколько минут..."

# Установка Go
brew install go

# Проверка установки
if command -v go &> /dev/null; then
    echo ""
    echo "✅ Go успешно установлен!"
    go version
    echo ""
    echo "📝 Настройка переменных окружения..."
    
    # Проверяем, нужно ли добавить в PATH
    if ! grep -q 'export GOPATH' ~/.zshrc 2>/dev/null; then
        echo "" >> ~/.zshrc
        echo "# Go environment variables" >> ~/.zshrc
        echo "export GOPATH=\$HOME/go" >> ~/.zshrc
        echo "export PATH=\$PATH:\$GOPATH/bin" >> ~/.zshrc
        echo "✅ Переменные окружения добавлены в ~/.zshrc"
        echo "⚠️  Выполните: source ~/.zshrc или перезапустите терминал"
    fi
    
    echo ""
    echo "🎉 Готово! Теперь вы можете запустить:"
    echo "   ./run_go_coverage.sh"
else
    echo "❌ Ошибка при установке Go"
    exit 1
fi

