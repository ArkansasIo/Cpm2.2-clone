# SDK and IDE Specification

## SDK
The SDK is a compact, reproducible development kit for CP/M and host-native applications. It contains project templates, assembler integration, C/C++ compiler integration, linker configuration, headers, libraries, examples, and test runners.

## Integrated IDEs
Two lightweight IDE profiles are planned:
- ASM Studio: Z80/8080 assembly editor, symbol browser, assembler output, hex view, debugger integration.
- C/C++ Studio: Tiny-C-style compact workflow, compiler diagnostics, project tree, build/run/debug controls, and cross-target toolchain selection.

The IDEs are front ends to toolchain adapters; they do not embed a compiler implementation unless a separately licensed compact compiler is intentionally included.

## Build targets
The project model supports CP/M guest binaries plus native Linux, Raspberry Pi ARM64, Windows, macOS, Android, and iOS targets where the corresponding toolchain is available.

## Reproducibility
Every build records product ID, version, build name, build ID, source revision, target architecture, compiler identity, and build timestamp.
