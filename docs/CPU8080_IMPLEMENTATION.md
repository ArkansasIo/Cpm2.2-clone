# Intel 8080 CPU implementation

The project now contains a deterministic, callback-backed Intel 8080 execution core suitable as the foundation for a CP/M machine.

## Implemented in this milestone

- 64 KiB address space callbacks
- Reset state
- NOP and HLT
- MVI register and MVI M
- LXI B/D/H/SP
- LDA and STA
- JMP
- ADD r/M
- ADC r/M
- INR/ DCR A
- Sign, zero, parity, auxiliary-carry and carry flags for the implemented arithmetic
- Cycle accounting for implemented instructions
- Deterministic halt on an unsupported opcode

## Architecture

`cpm22::memory::Memory` owns the 65,536-byte address space.

`cpm22::cpu::Cpu8080` is intentionally independent of the memory implementation. It receives read/write callbacks, making it suitable for later memory-mapped devices and CP/M BIOS integration.

`cpm22::cpm::Machine` binds the CPU to the 64 KiB memory implementation and provides reset, load, step, and bounded-run operations.

## Scope

This is **not yet a complete Intel 8080 emulator** and does not claim complete CP/M compatibility. The next instruction-set milestones should add register-pair operations, stack/call/return instructions, conditional branches, logical/compare operations, rotate operations, interrupt state, and exhaustive opcode/cycle tests.

## Verification

`tests/test_cpu8080.cpp` exercises immediate loads, register arithmetic, memory-indirect arithmetic, flags, HLT, and cycle accounting. The normal CTest build includes the regression test as `cpu8080_regression`.
