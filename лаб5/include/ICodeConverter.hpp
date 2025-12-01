#ifndef ICODECONVERTER_HPP
#define ICODECONVERTER_HPP

#include <string>

class ICodeConverter {
public:
    virtual ~ICodeConverter() = default;
    virtual std::string convert(const std::string& code) const = 0;
};

#endif // ICODECONVERTER_HPP
