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
