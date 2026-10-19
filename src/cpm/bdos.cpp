#include "cpm/bdos.hpp"
#include <algorithm>

namespace cpm22::cpm {

Bdos::Bdos(memory::Memory& memory) : memory_(memory) {}

void Bdos::set_console(ConsoleInput input, ConsoleOutput output) {
    input_ = std::move(input);
    output_ = std::move(output);
}

std::uint8_t Bdos::dispatch(std::uint8_t function, std::uint16_t de) {
    last_error_.clear();
    switch (function) {
    case 2: // console output
        if (output_) output_(static_cast<char>(de & 0xff));
        return 0;
    case 6: { // direct console I/O
        if (de == 0xff) return input_ ? static_cast<std::uint8_t>(std::max(input_(), 0)) : 0;
        if (output_) output_(static_cast<char>(de & 0xff));
        return 0;
    }
    case 9: { // print $-terminated string at DE
        for (std::uint16_t p = de; ; ++p) {
            const auto ch = memory_.read(p);
            if (ch == '$') break;
            if (output_) output_(static_cast<char>(ch));
        }
        return 0;
    }
    case 11: // console status
        return 0;
    default:
        last_error_ = "Unsupported BDOS function " + std::to_string(function);
        return 0xff;
    }
}

} // namespace cpm22::cpm
