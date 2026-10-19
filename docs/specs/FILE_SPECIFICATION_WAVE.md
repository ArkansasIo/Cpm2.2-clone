# File Specification Wave

This wave documents the next implementation boundaries before expanding source code.

- `src/core/`: application lifecycle, service registry, configuration, version/build metadata.
- `src/cpu/`: 8080/Z80 state, instruction execution, flags, interrupts, timing.
- `src/memory/`: guest memory map, access checks, snapshots.
- `src/cpm/`: BIOS, BDOS, CCP, boot and compatibility services.
- `src/filesystem/`: disk image, directory, allocation, serialization.
- `src/devices/`: console, storage, clock, input/output bus.
- `src/audio/`: event stream, Morse-like synthesis, WAV output.
- `src/gui/`: platform-independent UI models plus Linux/SDL renderers.
- `src/desktop/`: shell, panels, launcher, XMB, settings.
- `src/sdk/`: project model, toolchain adapters, assembler/compiler/debugger integration.
- `src/platform/`: host capability and platform adapters.
- `tests/`: unit, integration, deterministic replay, and packaging checks.

Each future source file receives a matching page under `docs/file-specs/` with purpose, API, dependencies, state ownership, tests, platform constraints, assets, and provenance.
