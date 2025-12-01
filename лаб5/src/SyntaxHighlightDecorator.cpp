#include "SyntaxHighlightDecorator.hpp"
#include <regex>

SyntaxHighlightDecorator::SyntaxHighlightDecorator(ICodeConverter* comp)
    : DecoratorConverter(comp) {}

std::string SyntaxHighlightDecorator::convert(const std::string& code) const {
    // сначала конвертируем базово → HTML
    std::string html = component->convert(code);

    // затем подсветка внутри <code>...</code>
    return highlightCpp(html);
}

std::string SyntaxHighlightDecorator::highlightCpp(const std::string& code) const {
    std::string result = code;

    // Ключевые слова C++
    static const std::vector<std::string> keywords = {
        "int", "float", "double", "char", "void", "return", "class", "struct",
        "if", "else", "switch", "case", "while", "for", "break", "continue",
        "public", "private", "protected", "virtual", "override", "const",
        "static", "include", "namespace", "using", "new", "delete", "auto",
        "template", "typename", "this"
    };

    for (const auto& kw : keywords) {
        std::regex rx("\\b" + kw + "\\b");
        result = std::regex_replace(result, rx,
            "<span style=\"color:#00aaff; font-weight:bold\">" + kw + "</span>");
    }

    // Строки
    {
        std::regex rx("\"([^\"\\\\]|\\\\.)*\"");
        result = std::regex_replace(result, rx,
            "<span style=\"color:#ff5500\">$&</span>");
    }

    // Однострочные комментарии //
    {
        std::regex rx("//.*");
        result = std::regex_replace(result, rx,
            "<span style=\"color:#55aa55\">$&</span>");
    }

    // Многострочные комментарии /* ... */
    {
        std::regex rx("/\\*[^*]*\\*+(?:[^/*][^*]*\\*+)*/");
        result = std::regex_replace(result, rx,
            "<span style=\"color:#55aa55\">$&</span>");
    }

    // Числа
    {
        std::regex rx("\\b[0-9]+(\\.[0-9]+)?\\b");
        result = std::regex_replace(result, rx,
            "<span style=\"color:#aa00aa\">$&</span>");
    }

    // Препроцессор
    {
        std::regex rx("#[a-zA-Z_]+");
        result = std::regex_replace(result, rx,
            "<span style=\"color:#ffaa00; font-weight:bold\">$&</span>");
    }

    return result;
}
