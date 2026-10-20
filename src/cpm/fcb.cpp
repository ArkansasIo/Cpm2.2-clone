#include "cpm/fcb.hpp"
#include <cctype>

namespace cpm22::cpm {

Fcb Fcb::from_string(const std::string& value, std::uint8_t user_id) {
    Fcb f;
    f.user = user_id;
    f.name.fill(' ');
    f.extension.fill(' ');

    // CP/M callers sometimes pass a glob pattern (for example, "A*.*").
    // A '*' in a field means all remaining characters in that field, so
    // expand it to '?' bytes to match the field's fixed width. Keep the
    // '*' itself only when it is the final character of a complete field
    // width, so strings such as "TOO-LONG-NAME.TXT" are not silently treated
    // as a valid shortened name.
    const auto dot = value.find('.');
    const auto n = value.substr(0, dot);
    const auto e = dot == std::string::npos ? std::string{} : value.substr(dot + 1);
    auto parse_field = [](const std::string& input, auto& output) {
        for (std::size_t i = 0; i < input.size() && i < output.size(); ++i) {
            const unsigned char ch = static_cast<unsigned char>(input[i]);
            if (input[i] == '*') {
                std::fill(output.begin() + static_cast<std::ptrdiff_t>(i), output.end(), '?');
                return;
            }
            output[i] = static_cast<char>(std::toupper(ch));
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
