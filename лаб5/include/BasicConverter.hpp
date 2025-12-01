#ifndef BASICCONVERTER_HPP
#define BASICCONVERTER_HPP

#include "ICodeConverter.hpp"

class BasicConverter : public ICodeConverter {
public:
    std::string convert(const std::string& code) const override;
};

#endif // BASICCONVERTER_HPP
