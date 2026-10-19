#pragma once
#include "cpu/cpu8080.hpp"
#include "memory/memory.hpp"
#include <cstddef>
#include <cstdint>
namespace cpm22::cpm {
class Machine {
public:
    Machine();
    void reset();
    std::uint8_t step();
    std::size_t run(std::size_t max_steps);
    void load(std::uint16_t address,const std::uint8_t* data,std::size_t size);
    [[nodiscard]] cpu::Cpu8080& cpu() noexcept { return cpu_; }
    [[nodiscard]] const cpu::Cpu8080& cpu() const noexcept { return cpu_; }
    [[nodiscard]] memory::Memory& memory() noexcept { return memory_; }
private:
    memory::Memory memory_;
    cpu::Cpu8080 cpu_;
};
}
