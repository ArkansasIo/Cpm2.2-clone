#include "cpm/bios.hpp"
#include <utility>

namespace cpm22::cpm {

Bios::Bios(memory::Memory& memory) : memory_(memory) {}

void Bios::set_console_output(ConsoleOutput output) { output_ = std::move(output); }

void Bios::cold_boot() {
    memory_.clear();
    current_drive_ = 0;
    if (output_) { output_('\r'); output_('\n'); }
}

void Bios::warm_boot() {
    if (output_) { output_('\r'); output_('\n'); }
}

} // namespace cpm22::cpm
