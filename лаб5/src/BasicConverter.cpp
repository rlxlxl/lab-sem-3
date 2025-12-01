#include "BasicConverter.hpp"
#include <string>

std::string BasicConverter::convert(const std::string& code) const {
    std::string html;

    html += "<pre><code>\n";

    for (char c : code) {
        switch (c) {
            case '<': html += "&lt;"; break;
            case '>': html += "&gt;"; break;
            case '&': html += "&amp;"; break;
            default:  html += c; break;
        }
    }

    html += "\n</code></pre>";

    return html;
}
