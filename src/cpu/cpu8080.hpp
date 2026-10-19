#pragma once

#include <array>
#include <cstdint>
#include <functional>

namespace cpm22::cpu {

struct Flags {
    bool sign = false;
    bool zero = true;
    bool auxiliary_carry = false;
    bool parity = true;
    bool carry = false;
};

struct Registers {
    std::uint8_t a = 0;
    std::uint8_t b = 0;
    std::uint8_t c = 0;
    std::uint8_t d = 0;
    std::uint8_t e = 0;
    std::uint8_t h = 0;
    std::uint8_t l = 0;
};

class Cpu8080 {
public:
    using MemoryRead = std::function<std::uint8_t(std::uint16_t)>;
    using MemoryWrite = std::function<void(std::uint16_t, std::uint8_t)>;

    explicit Cpu8080(MemoryRead read = {}, MemoryWrite write = {});

    void reset() noexcept;
    std::uint8_t step();

    [[nodiscard]] const Registers& registers() const noexcept { return regs_; }
    [[nodiscard]] const Flags& flags() const noexcept { return flags_; }
    [[nodiscard]] std::uint16_t program_counter() const noexcept { return pc_; }
    [[nodiscard]] std::uint16_t stack_pointer() const noexcept { return sp_; }
    [[nodiscard]] std::uint64_t cycles() const noexcept { return cycles_; }
    [[nodiscard]] bool halted() const noexcept { return halted_; }

    void set_program_counter(std::uint16_t value) noexcept { pc_ = value; }
    void set_stack_pointer(std::uint16_t value) noexcept { sp_ = value; }

private:
    Registers regs_{};
    Flags flags_{};
    std::uint16_t pc_ = 0;
    std::uint16_t sp_ = 0;
    std::uint64_t cycles_ = 0;
    bool halted_ = false;
    MemoryRead read_;
    MemoryWrite write_;

    std::uint8_t read8(std::uint16_t address) const;
    void write8(std::uint16_t address, std::uint8_t value) const;
    std::uint16_t read16(std::uint16_t address) const;
    void write16(std::uint16_t address, std::uint16_t value) const;
    void update_szp(std::uint8_t value) noexcept;
    void add(std::uint8_t value, bool with_carry) noexcept;
};

} // namespace cpm22::cpu
