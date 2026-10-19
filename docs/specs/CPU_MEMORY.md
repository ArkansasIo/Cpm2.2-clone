# CPU and Memory Specification

## CPU
The guest CPU layer shall expose a common interface for the baseline Intel 8080-compatible instruction set and Z80 extensions. The initial public model includes registers, flags, program counter, stack pointer, instruction fetch/decode/execute, interrupt state, cycle accounting, and reset.

## Memory
The baseline guest memory model is 64 KiB byte-addressable RAM. Memory access is bounds-safe, deterministic, and observable by the debugger. ROM/boot regions and device-mapped ranges are represented as explicit mappings rather than hidden special cases.

## Debugger contract
A debugger can inspect registers, memory, flags, program counter, stack, breakpoints, watchpoints, and execution counters without changing state unless a write operation is explicitly requested.

## Verification
Instruction tests use known input states and expected output states. Memory tests cover boundary addresses, reset initialization, read/write symmetry, and protected mappings.

## Status
Design specification; CPU implementation is not considered complete until the regression suite passes on the supported host toolchains.
