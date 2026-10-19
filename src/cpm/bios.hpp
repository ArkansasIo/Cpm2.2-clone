#pragma once
#include "memory/memory.hpp"
#include <cstdint>
#include <functional>

namespace cpm22::cpm {

class Bios {
public:
    using ConsoleOutput = std::function<void(char)>;
    explicit Bios(memory::Memory& memory);
    void cold_boot();
    void warm_boot();
    void set_console_output(ConsoleOutput output);
    [[nodiscard]] std::uint8_t current_drive() const noexcept { return current_drive_; }
    void select_drive(std::uint8_t drive) noexcept { current_drive_ = drive & 0x0f; }

private:
    memory::Memory& memory_;
    ConsoleOutput output_;
    std::uint8_t current_drive_ = 0;
};

} // namespace cpm22::cpm
