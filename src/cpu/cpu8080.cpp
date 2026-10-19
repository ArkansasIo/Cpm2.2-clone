#include "cpu8080.hpp"

namespace cpm22::cpu {
namespace {

bool parity_even(std::uint8_t value) {
    bool parity = true;
    while (value) {
        parity = !parity;
        value &= static_cast<std::uint8_t>(value - 1);
    }
    return parity;
}

std::uint8_t& reg_by_code(Registers& r, unsigned code) {
    switch (code & 7u) {
    case 0: return r.b;
    case 1: return r.c;
    case 2: return r.d;
    case 3: return r.e;
    case 4: return r.h;
    case 5: return r.l;
    default: return r.a;
    }
}

} // namespace

Cpu8080::Cpu8080(MemoryRead read, MemoryWrite write)
    : read_(std::move(read)), write_(std::move(write)) {}

void Cpu8080::reset() noexcept {
    regs_ = {};
    flags_ = {};
    pc_ = 0;
    sp_ = 0;
    cycles_ = 0;
    halted_ = false;
}

std::uint8_t Cpu8080::read8(std::uint16_t address) const {
    return read_ ? read_(address) : 0;
}

void Cpu8080::write8(std::uint16_t address, std::uint8_t value) const {
    if (write_) write_(address, value);
}

std::uint16_t Cpu8080::read16(std::uint16_t address) const {
    const auto lo = read8(address);
    const auto hi = read8(static_cast<std::uint16_t>(address + 1));
    return static_cast<std::uint16_t>(lo | (static_cast<std::uint16_t>(hi) << 8));
}

void Cpu8080::write16(std::uint16_t address, std::uint16_t value) const {
    write8(address, static_cast<std::uint8_t>(value & 0xff));
    write8(static_cast<std::uint16_t>(address + 1), static_cast<std::uint8_t>(value >> 8));
}

void Cpu8080::update_szp(std::uint8_t value) noexcept {
    flags_.sign = (value & 0x80u) != 0;
    flags_.zero = value == 0;
    flags_.parity = parity_even(value);
}

void Cpu8080::add(std::uint8_t value, bool with_carry) noexcept {
    const unsigned carry = with_carry && flags_.carry ? 1u : 0u;
    const unsigned result = static_cast<unsigned>(regs_.a) + value + carry;
    flags_.auxiliary_carry = ((regs_.a & 0x0fu) + (value & 0x0fu) + carry) > 0x0fu;
    flags_.carry = result > 0xffu;
    regs_.a = static_cast<std::uint8_t>(result);
    update_szp(regs_.a);
}

std::uint8_t Cpu8080::step() {
    if (halted_) return 0;

    const auto opcode = read8(pc_++);
    switch (opcode) {
    case 0x00: // NOP
        cycles_ += 4;
        return 4;

    case 0x76: // HLT
        halted_ = true;
        cycles_ += 7;
        return 7;

    case 0x3e: // MVI A,imm
        regs_.a = read8(pc_++);
        cycles_ += 7;
        return 7;

    case 0x06: // MVI B,imm
    case 0x0e: // MVI C,imm
    case 0x16: // MVI D,imm
    case 0x1e: // MVI E,imm
    case 0x26: // MVI H,imm
    case 0x2e: { // MVI L,imm
        reg_by_code(regs_, (opcode >> 3u) & 7u) = read8(pc_++);
        cycles_ += 7;
        return 7;
    }

    case 0x32: { // STA addr
        const auto address = read16(pc_);
        pc_ = static_cast<std::uint16_t>(pc_ + 2);
        write8(address, regs_.a);
        cycles_ += 13;
        return 13;
    }

    case 0x3a: { // LDA addr
        const auto address = read16(pc_);
        pc_ = static_cast<std::uint16_t>(pc_ + 2);
        regs_.a = read8(address);
        cycles_ += 13;
        return 13;
    }

    case 0x80: case 0x81: case 0x82: case 0x83:
    case 0x84: case 0x85: case 0x87:
        add(reg_by_code(regs_, opcode & 7u), false);
        cycles_ += 4;
        return 4;

    case 0x88: case 0x89: case 0x8a: case 0x8b:
    case 0x8c: case 0x8d: case 0x8f:
        add(reg_by_code(regs_, opcode & 7u), true);
        cycles_ += 4;
        return 4;

    case 0xc3: // JMP addr
        pc_ = read16(pc_);
        cycles_ += 10;
        return 10;

    case 0x31: // LXI SP,imm16
        sp_ = read16(pc_);
        pc_ = static_cast<std::uint16_t>(pc_ + 2);
        cycles_ += 10;
        return 10;

    default:
        // Unsupported opcodes are treated as a deterministic one-step trap
        // for this incremental implementation rather than silently skipped.
        halted_ = true;
        cycles_ += 4;
        return 4;
    }
}

} // namespace cpm22::cpu
