#include "cpm/bdos.hpp"
#include "cpm/bios.hpp"
#include "cpm/cpm_machine.hpp"
#include <iostream>

int main() {
    cpm22::cpm::Machine machine;
    cpm22::cpm::Bios bios(machine.memory());
    cpm22::cpm::Bdos bdos(machine.memory());

    bios.set_console_output([](char ch) { std::cout << ch << std::flush; });
    bdos.set_console({}, [](char ch) { std::cout << ch << std::flush; });
    bios.cold_boot();

    std::cout << "CP/M 2.2 Universal Development Environment\r\n";
    std::cout << "8080 emulator online.\r\n";
    std::cout << "Type HELP for host commands; CP/M execution layer is under development.\r\n";
    return 0;
}
