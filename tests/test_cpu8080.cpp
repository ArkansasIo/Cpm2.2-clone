#include "cpu/cpu8080.hpp"
#include <array>
#include <cassert>
#include <cstdint>
#include <iostream>
int main(){
 std::array<std::uint8_t,65536> m{};
 cpm22::cpu::Cpu8080 cpu([&](std::uint16_t a){return m[a];},[&](std::uint16_t a,std::uint8_t v){m[a]=v;});
 m[0]=0x3e;m[1]=5;m[2]=0x06;m[3]=7;m[4]=0x80;m[5]=0x76;
 assert(cpu.step()==7&&cpu.registers().a==5); assert(cpu.step()==7&&cpu.registers().b==7); assert(cpu.step()==4&&cpu.registers().a==12); assert(cpu.flags().parity&&!cpu.flags().zero); assert(cpu.step()==7&&cpu.halted()); assert(cpu.cycles()==25);
 cpu.reset();m.fill(0); m[0]=0x21;m[1]=0x00;m[2]=0x20;m[3]=0x3e;m[4]=5;m[5]=0x36;m[6]=7;m[7]=0x86;m[8]=0x76;
 cpu.step();cpu.step();cpu.step();assert(m[0x2000]==7);assert(cpu.step()==7&&cpu.registers().a==12);cpu.step();assert(cpu.halted());
 cpu.reset();m.fill(0);m[0]=0x3e;m[1]=0xff;m[2]=0x06;m[3]=1;m[4]=0x80;m[5]=0x76;cpu.step();cpu.step();cpu.step();assert(cpu.registers().a==0&&cpu.flags().carry&&cpu.flags().zero&&cpu.flags().parity);cpu.step();
 std::cout<<"8080 CPU tests passed\n"; return 0;
}
