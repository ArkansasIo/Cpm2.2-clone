# CP/M Universal Development Environment — Documentation Map

## Purpose

This document defines the documentation and asset contract for the project. Documentation is created before implementation expands so that every subsystem has a stable architectural reference.

## Documentation hierarchy

```text
CP/M Universal Development Environment
│
├── Product
│   ├── Vision
│   ├── Requirements
│   ├── Scope
│   └── Release/Versioning
│
├── Architecture
│   ├── System Context
│   ├── UML Component Model
│   ├── UML Package Model
│   ├── UML Class Model
│   ├── Runtime/Data Flow
│   ├── Threading
│   └── Portability
│
├── Core
│   ├── CPU
│   │   ├── Z80
│   │   └── 8080
│   ├── Memory
│   ├── Machine
│   └── Execution
│
├── CP/M
│   ├── CCP
│   ├── BDOS
│   ├── BIOS
│   ├── FCB
│   ├── Filesystem
│   └── Boot
│
├── Devices
│   ├── Storage
│   ├── Console
│   ├── Printer
│   ├── RTC/CMOS
│   └── Audio Events
│
├── UI/Desktop
│   ├── GUI
│   ├── Desktop Shell
│   ├── XMB
│   ├── File Manager
│   ├── Terminal
│   └── Settings
│
├── SDK
│   ├── CP/M SDK
│   ├── Z80 SDK
│   ├── 8080 SDK
│   ├── Firmware SDK
│   ├── Audio SDK
│   └── GUI SDK
│
├── Platforms
│   ├── Linux
│   ├── Windows
│   ├── macOS
│   ├── Android
│   ├── iOS
│   └── Raspberry Pi 3/4/5
│
├── Updates
│   ├── Firmware
│   ├── OS
│   ├── Combined
│   ├── Verification
│   └── Rollback
│
└── Verification
    ├── Unit Tests
    ├── Integration Tests
    ├── Compatibility Tests
    ├── Platform Tests
    └── Release Tests
```

## File-level documentation rule

Each implementation module should have:

1. A module overview in `docs/`.
2. API documentation for public classes/functions.
3. A dependency list.
4. A UML relationship entry where applicable.
5. Test coverage references.
6. Platform constraints.
7. Asset references where assets are required.
8. License/provenance references for external material.

## UML conventions

- Node = subsystem, module, class, file, asset, test, or external dependency.
- Branch = ownership or dependency relationship.
- Sub-node = public API, data structure, configuration, test fixture, or implementation detail.
- Dashed relationship = optional/platform-specific dependency.
- Solid relationship = required runtime dependency.
- External dependency nodes are isolated under `third_party/` and documented in `THIRD_PARTY.md`.

## Reference sources

The implementation should rely on public technical specifications and independently written compatibility code. Historical CP/M behavior is reproduced from documented interfaces and observed behavior rather than copied proprietary source.

Primary reference categories:

- CP/M-compatible public interface descriptions
- Z80 and Intel 8080 programmer documentation
- ISO C++23 language/library specifications
- CMake documentation
- Platform SDK documentation
- Android NDK documentation
- Linux/POSIX documentation
- Apple platform documentation
- Microsoft Windows development documentation
- Raspberry Pi/Linux hardware documentation

Exact external sources used for implementation decisions must be recorded in the applicable module document and, when code/assets are reused, in `THIRD_PARTY.md`.
