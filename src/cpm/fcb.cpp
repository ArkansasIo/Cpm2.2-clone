#include "cpm/fcb.hpp"
#include <cctype>

namespace cpm22::cpm {

Fcb Fcb::from_string(const std::string& value, std::uint8_t user_id) {
    Fcb f;
    f.user = user_id;
    f.name.fill(' ');
    f.extension.fill(' ');
    const auto dot = value.find('.');
    const auto n = value.substr(0, dot);
    const auto e = dot == std::string::npos ? std::string{} : value.substr(dot + 1);
    for (std::size_t i = 0; i < n.size() && i < f.name.size(); ++i)
        f.name[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(n[i])));
    for (std::size_t i = 0; i < e.size() && i < f.extension.size(); ++i)
        f.extension[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(e[i])));
    return f;
}

std::string Fcb::filename() const {
    std::string n, e;
    for (char c : name) if (c != ' ' && c != '\0') n += c;
    for (char c : extension) if (c != ' ' && c != '\0') e += c;
    return e.empty() ? n : n + "." + e;
}

} // namespace cpm22::cpm
