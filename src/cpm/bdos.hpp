#pragma once
#include "cpm/directory.hpp"
#include "memory/memory.hpp"
#include <cstdint>
#include <functional>
#include <string>

namespace cpm22::cpm {

class Bdos {
public:
    using ConsoleInput = std::function<int()>;
    using ConsoleOutput = std::function<void(char)>;

    explicit Bdos(memory::Memory& memory);
    void set_console(ConsoleInput input, ConsoleOutput output);
    std::uint8_t dispatch(std::uint8_t function, std::uint16_t de);
    [[nodiscard]] Directory& directory() noexcept { return directory_; }
    [[nodiscard]] const std::string& last_error() const noexcept { return last_error_; }

private:
    memory::Memory& memory_;
    Directory directory_;
    ConsoleInput input_;
    ConsoleOutput output_;
    std::string last_error_;
};

} // namespace cpm22::cpm
