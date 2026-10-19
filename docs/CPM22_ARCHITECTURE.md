# CP/M 2.2 Emulator Architecture

## Scope

This repository implements a clean-room, testable CP/M 2.2-style environment. It does not copy historical Digital Research source code.

## Layers

```text
Host Application
      |
      v
+------------------+
| Console / GUI    |
+------------------+
      |
+------------------+
| CCP / Command    |
| Environment      |
+------------------+
      |
+------------------+
| BDOS Services    |
+------------------+
      |
+------------------+
| BIOS Abstraction |
+------------------+
      |
+------------------+
| Storage / Devices|
+------------------+
      |
+------------------+
| 8080 CPU + RAM   |
+------------------+
```

## Current implementation

- 64 KiB address-space model.
- Intel 8080 CPU component.
- Virtual storage/device abstraction.
- Directory model using CP/M-compatible 8.3 naming constraints.
- Initial BDOS console functions 2, 6, 9, and 11.
- BIOS cold/warm boot abstraction.
- CLI development application.
- CTest coverage for CPU, audio, directory, and BDOS layers.

## Compatibility roadmap

1. Complete the 8080 instruction and flag matrix.
2. Add Z80 execution as an optional CPU backend.
3. Implement a disk parameter block and sector deblocking layer.
4. Add FCB parsing and file operations.
5. Implement CCP command parsing and transient-program loading.
6. Add a configurable BIOS device table.
7. Add boot-image generation and reproducible disk-image tooling.
8. Build conformance tests against published CP/M behavior specifications.

## Design rule

Hardware-specific behavior stays behind interfaces. The core emulator must remain deterministic and host-independent so that Linux, Windows, macOS, and embedded frontends can reuse the same machine model.
