#include "cpm/fcb.hpp"
#include <cctype>

namespace cpm22::cpm {

bool Fcb::is_valid_filename(const std::string& value) noexcept {
    if (value.empty()) return false;

    const auto dot = value.find('.');
    if (dot != std::string::npos && value.find('.', dot + 1) != std::string::npos) {
        return false;
    }

    const std::string name = value.substr(0, dot);
    const std::string extension =
        dot == std::string::npos ? std::string{} : value.substr(dot + 1);
    if (name.empty() || name.size() > 8 || extension.size() > 3) {
        return false;
    }

    const auto valid_field = [](const std::string& field) {
        for (unsigned char c : field) {
            // CP/M file fields cannot contain spaces, controls, non-ASCII
            // bytes or filename metacharacters.
            if (c <= 0x20 || c >= 0x7F || c == '*' || c == '?' ||
                c == '/' || c == '\\' || c == ':' || c == ';' ||
                c == ',' || c == '[' || c == ']' || c == '=' ||
                c == '<' || c == '>' || c == '|') {
                return false;
            }
        }
        return true;
    };

    return valid_field(name) && valid_field(extension);
}

Fcb Fcb::from_string(const std::string& value, std::uint8_t user_id) {
    Fcb f;
    f.user = user_id;
    f.name.fill(' ');
    f.extension.fill(' ');

    const auto dot = value.find('.');
    const auto n = value.substr(0, dot);
    const auto e = dot == std::string::npos ? std::string{} : value.substr(dot + 1);
    auto parse_field = [](const std::string& input, auto& output) {
        const auto star = input.find('*');
        if (star != std::string::npos) {
            // Validate wildcard placement before expansion so malformed
            // names are not accepted after fixed-width truncation.
            for (std::size_t i = 0; i < star; ++i) {
                output[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(input[i])));
            }
            if (star < output.size()) {
                output[star] = '*';
                for (std::size_t i = star + 1; i < output.size(); ++i) output[i] = '?';
            }
            return;
        }
        for (std::size_t i = 0; i < input.size() && i < output.size(); ++i) {
            output[i] = static_cast<char>(std::toupper(static_cast<unsigned char>(input[i])));
        }
    };
    parse_field(n, f.name);
    parse_field(e, f.extension);
    return f;
}

std::string Fcb::filename() const {
    std::string n, e;
    for (char c : name) if (c != ' ' && c != '\0') n += c;
    for (char c : extension) if (c != ' ' && c != '\0') e += c;
    return e.empty() ? n : n + "." + e;
}

} // namespace cpm22::cpm
