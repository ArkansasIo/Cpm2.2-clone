#include "cpm/fcb.hpp"
#include <cctype>

namespace cpm22::cpm {

Fcb Fcb::from_string(const std::string& value, std::uint8_t user_id) {
    Fcb f;
    f.user = user_id;
    f.name.fill(' ');
    f.extension.fill(' ');
    const auto dot = value.find('.');
    auto n = value.substr(0, dot);
    auto e = dot == std::string::npos ? std::string{} : value.substr(dot + 1);

    // CP/M FCB names are fixed-width fields and support '?' and '*' wildcards.
    // Expand '*' to the remaining field width so filename() and directory
    // matching preserve classic 8.3 wildcard semantics.
    const auto normalize_field = [](std::string field, std::size_t width) {
        for (auto& c : field)
            c = static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
        const auto star = field.find('*');
        if (star != std::string::npos) {
            field.resize(star);
            field.append(width - field.size(), '?');
        }
        if (field.size() > width) field.resize(width);
        return field;
    };
    n = normalize_field(std::move(n), f.name.size());
    e = normalize_field(std::move(e), f.extension.size());
    for (std::size_t i = 0; i < n.size(); ++i)
        f.name[i] = n[i];
    for (std::size_t i = 0; i < e.size(); ++i)
        f.extension[i] = e[i];
    return f;
}

std::string Fcb::filename() const {
    std::string n, e;
    for (char c : name) if (c != ' ' && c != '\0') n += c;
    for (char c : extension) if (c != ' ' && c != '\0') e += c;
    return e.empty() ? n : n + "." + e;
}

} // namespace cpm22::cpm
