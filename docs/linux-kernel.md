# Linux kernel integration

The emulator has an optional Linux-kernel integration layer, but it does **not** pretend that a small compatibility wrapper is the Linux kernel itself.

## What is included

- `src/platform/linux_kernel_layer.hpp/.cpp`: kernel boot contract and configuration.
- `boot/cpm_bootloader.asm`: CP/M-style firmware boot path.
- A clean separation between the CP/M 2.2 machine and an optional Linux guest.

## Full Linux source

The complete upstream Linux source tree is intentionally not copied into this repository as an opaque generated dump. When a real Linux guest is enabled, supply a specific kernel release as a tracked submodule or separately versioned source artifact, then connect its boot image to the Linux layer.

This keeps the CP/M emulator source maintainable and makes the exact Linux version reproducible.

## Target architecture

```text
CP/M CPU + BIOS + BDOS + CCP
          |
          +---- CMOS/RTC
          |
          +---- Storage -> Morse/retro audio
          |
          +---- CP/M-style bootloader
          |
          +---- optional Linux kernel guest
                         |
                    Linux boot ABI
```
