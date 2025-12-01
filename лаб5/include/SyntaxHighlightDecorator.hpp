#ifndef SYNTAXHIGHLIGHTDECORATOR_HPP
#define SYNTAXHIGHLIGHTDECORATOR_HPP

#include "DecoratorConverter.hpp"
#include <regex>
#include <vector>

class SyntaxHighlightDecorator : public DecoratorConverter {
public:
    explicit SyntaxHighlightDecorator(ICodeConverter* comp);

    std::string convert(const std::string& code) const override;

private:
    std::string highlightCpp(const std::string& code) const;

    std::string color(const std::string& text, const std::string& htmlColor) const {
        return "<span style=\"color:" + htmlColor + "\">" + text + "</span>";
    }
};

#endif // SYNTAXHIGHLIGHTDECORATOR_HPP
