# CP/M-style boot layer

`cpm_bootloader.asm` defines the clean-room boot protocol used by this emulator. It initializes the CPU, asks the emulator BIOS to select/read the boot media, reports a boot error, and transfers control to the CCP load address.

This is intentionally an original compatibility implementation. It does not vendor or reproduce the historical CP/M source tree.

## Linux option

The emulator can expose an optional Linux boot contract alongside CP/M. A real Linux kernel image remains a separately supplied artifact; the repository's `src/platform/linux_kernel_layer.*` models the integration boundary rather than falsely claiming to contain the entire upstream Linux kernel.
