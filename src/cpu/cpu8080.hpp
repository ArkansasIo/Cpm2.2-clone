#pragma once

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
    std::uint8_t a{}, b{}, c{}, d{}, e{}, h{}, l{};
};

class Cpu8080 {
public:
    using MemoryRead = std::function<std::uint8_t(std::uint16_t)>;
    using MemoryWrite = std::function<void(std::uint16_t, std::uint8_t)>;

    Cpu8080(MemoryRead read = {}, MemoryWrite write = {});
    void reset() noexcept;
    std::uint8_t step();

    [[nodiscard]] const Registers& registers() const noexcept { return r_; }
    [[nodiscard]] const Flags& flags() const noexcept { return f_; }
    [[nodiscard]] std::uint16_t pc() const noexcept { return pc_; }
    [[nodiscard]] std::uint16_t sp() const noexcept { return sp_; }
    [[nodiscard]] std::uint64_t cycles() const noexcept { return cycles_; }
    [[nodiscard]] bool halted() const noexcept { return halted_; }

    void set_pc(std::uint16_t v) noexcept { pc_ = v; }
    void set_sp(std::uint16_t v) noexcept { sp_ = v; }
    void set_a(std::uint8_t v) noexcept { r_.a = v; }

private:
    Registers r_{};
    Flags f_{};
    std::uint16_t pc_{};
    std::uint16_t sp_{};
    std::uint64_t cycles_{};
    bool halted_{};
    MemoryRead read_;
    MemoryWrite write_;

    std::uint8_t rd(std::uint16_t) const;
    void wr(std::uint16_t, std::uint8_t) const;
    std::uint16_t rd16(std::uint16_t) const;
    void set_szp(std::uint8_t) noexcept;
    void add(std::uint8_t, bool) noexcept;
    std::uint8_t& reg(unsigned) noexcept;
    std::uint8_t operand(unsigned);
};

} // namespace cpm22::cpu
