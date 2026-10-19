#include "cpm_machine.hpp"
#include <algorithm>
namespace cpm22::cpm {
Machine::Machine() : cpu_([this](std::uint16_t a){return memory_.read(a);},[this](std::uint16_t a,std::uint8_t v){memory_.write(a,v);}) { reset(); }
void Machine::reset(){ memory_.clear(); cpu_.reset(); }
std::uint8_t Machine::step(){ return cpu_.step(); }
std::size_t Machine::run(std::size_t max_steps){ std::size_t n=0; while(n<max_steps && !cpu_.halted()){ cpu_.step(); ++n; } return n; }
void Machine::load(std::uint16_t address,const std::uint8_t* data,std::size_t size){ const auto count=std::min(size,memory::Memory::size-static_cast<std::size_t>(address)); std::copy_n(data,count,memory_.data()+address); }
}
