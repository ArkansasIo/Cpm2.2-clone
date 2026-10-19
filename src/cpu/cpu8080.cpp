#include "cpu8080.hpp"
#include <utility>

namespace cpm22::cpu {
namespace { bool even(std::uint8_t v){ bool p=true; while(v){p=!p; v&=static_cast<std::uint8_t>(v-1);} return p; } }

Cpu8080::Cpu8080(MemoryRead read, MemoryWrite write) : read_(std::move(read)), write_(std::move(write)) {}
void Cpu8080::reset() noexcept { r_={}; f_={}; pc_=sp_=0; cycles_=0; halted_=false; }
std::uint8_t Cpu8080::rd(std::uint16_t a) const { return read_ ? read_(a) : 0; }
void Cpu8080::wr(std::uint16_t a,std::uint8_t v) const { if(write_) write_(a,v); }
std::uint16_t Cpu8080::rd16(std::uint16_t a) const { return static_cast<std::uint16_t>(rd(a) | (static_cast<std::uint16_t>(rd(static_cast<std::uint16_t>(a+1)))<<8)); }
void Cpu8080::set_szp(std::uint8_t v) noexcept { f_.sign=(v&0x80)!=0; f_.zero=v==0; f_.parity=even(v); }
void Cpu8080::add(std::uint8_t v,bool carry) noexcept { const unsigned c=carry&&f_.carry; const unsigned x=r_.a+v+c; f_.auxiliary_carry=((r_.a&15)+(v&15)+c)>15; f_.carry=x>255; r_.a=static_cast<std::uint8_t>(x); set_szp(r_.a); }
std::uint8_t& Cpu8080::reg(unsigned n) noexcept { switch(n&7u){case 0:return r_.b;case 1:return r_.c;case 2:return r_.d;case 3:return r_.e;case 4:return r_.h;case 5:return r_.l;default:return r_.a;} }
std::uint8_t Cpu8080::operand(unsigned n){ return (n&7u)==6 ? rd(static_cast<std::uint16_t>((r_.h<<8)|r_.l)) : reg(n); }

std::uint8_t Cpu8080::step(){
    if(halted_) return 0;
    const auto op=rd(pc_++);
    auto imm8=[&](){return rd(pc_++);};
    switch(op){
    case 0x00: cycles_+=4; return 4;
    case 0x76: halted_=true; cycles_+=7; return 7;
    case 0x3e: r_.a=imm8(); cycles_+=7; return 7;
    case 0x06: case 0x0e: case 0x16: case 0x1e: case 0x26: case 0x2e: reg((op>>3)&7)=imm8(); cycles_+=7; return 7;
    case 0x36: wr(static_cast<std::uint16_t>((r_.h<<8)|r_.l),imm8()); cycles_+=10; return 10;
    case 0x3a: { const auto a=rd16(pc_); pc_=static_cast<std::uint16_t>(pc_+2); r_.a=rd(a); cycles_+=13; return 13; }
    case 0x32: { const auto a=rd16(pc_); pc_=static_cast<std::uint16_t>(pc_+2); wr(a,r_.a); cycles_+=13; return 13; }
    case 0xc3: pc_=rd16(pc_); cycles_+=10; return 10;
    case 0x31: sp_=rd16(pc_); pc_=static_cast<std::uint16_t>(pc_+2); cycles_+=10; return 10;
    case 0x21: { const auto v=rd16(pc_); pc_=static_cast<std::uint16_t>(pc_+2); r_.h=static_cast<std::uint8_t>(v>>8); r_.l=static_cast<std::uint8_t>(v); cycles_+=10; return 10; }
    case 0x01: { const auto v=rd16(pc_); pc_=static_cast<std::uint16_t>(pc_+2); r_.b=static_cast<std::uint8_t>(v>>8); r_.c=static_cast<std::uint8_t>(v); cycles_+=10; return 10; }
    case 0x11: { const auto v=rd16(pc_); pc_=static_cast<std::uint16_t>(pc_+2); r_.d=static_cast<std::uint8_t>(v>>8); r_.e=static_cast<std::uint8_t>(v); cycles_+=10; return 10; }
    case 0x80: case 0x81: case 0x82: case 0x83: case 0x84: case 0x85: case 0x86: case 0x87: add(operand(op&7),false); cycles_+=((op&7)==6?7:4); return static_cast<std::uint8_t>((op&7)==6?7:4);
    case 0x88: case 0x89: case 0x8a: case 0x8b: case 0x8c: case 0x8d: case 0x8e: case 0x8f: add(operand(op&7),true); cycles_+=((op&7)==6?7:4); return static_cast<std::uint8_t>((op&7)==6?7:4);
    case 0x3c: ++r_.a; set_szp(r_.a); cycles_+=5; return 5;
    case 0x3d: --r_.a; set_szp(r_.a); cycles_+=5; return 5;
    default: halted_=true; cycles_+=4; return 4;
    }
}
} // namespace cpm22::cpu
