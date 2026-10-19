# Documentation

This directory is the documentation-first specification for the CP/M Universal Development Environment.

Documentation is organized before broad implementation. Each subsystem specification records purpose, interfaces, dependencies, tests, platform constraints, assets, and provenance.

## Sections

- `architecture/` — system context, data flow, memory map, threading, and package relationships.
- `core/` — machine, execution, configuration, and runtime contracts.
- `cpu/` — Z80/8080 CPU behavior, registers, flags, decoding, interrupts, and timing.
- `memory/` — 64 KiB guest address space and memory mapping.
- `cpm/` — CCP, BDOS, BIOS, FCB, directories, allocation, drives, and boot.
- `filesystem/` — disk images, sectors, file I/O, and persistence.
- `devices/` — console, storage, printer, RTC, and device events.
- `audio/` — storage-event audio and WAV behavior.
- `gui/`, `desktop/`, `xmb/` — graphical shell and navigation specifications.
- `sdk/` — CP/M, CPU, firmware, audio, GUI, assembler, and C/C++ tooling.
- `platforms/` — Linux, Windows, macOS, Android, iOS, and Raspberry Pi targets.
- `firmware/`, `os/`, `update/` — firmware, guest OS, and update/rollback contracts.
- `tests/` — verification strategy and compatibility testing.
- `packaging/` — target packaging and release requirements.
- `assets/` — runtime/test asset policy.

## Status vocabulary

- **Planned** — specified but not implemented.
- **Scaffolded** — source structure exists but behavior is incomplete.
- **Implemented** — behavior exists and has tests.
- **Verified** — implementation has passed the applicable verification suite.

The documentation must not label a feature as implemented or verified merely because its specification exists.

## File-level specification rule

Every implementation file will be mapped to a specification page under `docs/file-specs/` during the documentation-first pass. Each page records path, purpose, status, public API, dependencies, tests, platform constraints, assets, and provenance.
