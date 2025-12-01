#ifndef DECORATORCONVERTER_HPP
#define DECORATORCONVERTER_HPP

#include "ICodeConverter.hpp"

class DecoratorConverter : public ICodeConverter {
protected:
    ICodeConverter* component;

public:
    explicit DecoratorConverter(ICodeConverter* comp)
        : component(comp) {}

    std::string convert(const std::string& code) const override {
        return component->convert(code);
    }

    virtual ~DecoratorConverter() = default;
};

#endif // DECORATORCONVERTER_HPP
