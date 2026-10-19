#include "cpm/bdos.hpp"
#include "memory/memory.hpp"
#include <cassert>
#include <string>

int main() {
    cpm22::memory::Memory memory;
    cpm22::cpm::Bdos bdos(memory);
    std::string output;
    bdos.set_console({}, [&](char ch) { output.push_back(ch); });

    memory.write(0x0200, 'O');
    memory.write(0x0201, 'K');
    memory.write(0x0202, '$');
    assert(bdos.dispatch(9, 0x0200) == 0);
    assert(output == "OK");

    output.clear();
    assert(bdos.dispatch(2, 'X') == 0);
    assert(output == "X");
    assert(bdos.dispatch(255, 0) == 0xff);
    return 0;
}
