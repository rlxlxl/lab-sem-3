#include <iostream>
#include <fstream>
#include <sstream>

#include "BasicConverter.hpp"
#include "SyntaxHighlightDecorator.hpp"

int main() {
    std::string path;
    std::cout << "Введите путь к C++ файлу: ";
    std::getline(std::cin, path);

    std::ifstream file(path);
    if (!file) {
        std::cerr << "Ошибка: не удалось открыть файл.\n";
        return 1;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string code = buffer.str();

    // базовый конвертер
    BasicConverter basic;

    // декоратор подсветки
    SyntaxHighlightDecorator decorated(&basic);

    // преобразуем
    std::string html = decorated.convert(code);

    // сохраняем в файл
    std::ofstream out("output.html");
    out << html;

    std::cout << "Готово! Результат: output.html\n";

    return 0;
}
