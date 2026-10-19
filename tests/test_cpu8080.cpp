#include "cpu/cpu8080.hpp"

#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>

using namespace cpm22::cpu;

int main() {
    std::array<std::uint8_t, 65536> memory{};
    Cpu8080 cpu(
        [&](std::uint16_t address) { return memory[address]; },
        [&](std::uint16_t address, std::uint8_t value) { memory[address] = value; });

    // MVI A,5; MVI B,7; ADD B; HLT.
    memory[0x0000] = 0x3e;
    memory[0x0001] = 0x05;
    memory[0x0002] = 0x06;
    memory[0x0003] = 0x07;
    memory[0x0004] = 0x80;
    memory[0x0005] = 0x76;

    assert(cpu.step() == 7);
    assert(cpu.registers().a == 5);
    assert(cpu.step() == 7);
    assert(cpu.registers().b == 7);
    assert(cpu.step() == 4);
    assert(cpu.registers().a == 12);
    assert(!cpu.flags().zero);
    assert(cpu.flags().parity);
    assert(cpu.step() == 7);
    assert(cpu.halted());
    assert(cpu.cycles() == 25);

    cpu.reset();
    memory.fill(0);

    // LXI SP,0x4000; MVI A,0xaa; STA 0x2000; LDA 0x2000; HLT.
    memory[0] = 0x31; memory[1] = 0x00; memory[2] = 0x40;
    memory[3] = 0x3e; memory[4] = 0xaa;
    memory[5] = 0x32; memory[6] = 0x00; memory[7] = 0x20;
    memory[8] = 0x3a; memory[9] = 0x00; memory[10] = 0x20;
    memory[11] = 0x76;

    cpu.step();
    assert(cpu.stack_pointer() == 0x4000);
    cpu.step();
    cpu.step();
    assert(memory[0x2000] == 0xaa);
    cpu.step();
    assert(cpu.registers().a == 0xaa);
    cpu.step();
    assert(cpu.halted());

    std::cout << "8080 CPU regression tests passed\n";
    return 0;
}
