# CP/M Universal Development Environment

A clean-room C++23 project for a CP/M 2.2-style emulator and portable retro-computing development environment.

> Compatibility is implemented incrementally from published behavior/specification material. This repository is not the historical Digital Research source tree.

## Architecture

- **8080 CPU:** instruction execution and machine-state model.
- **Memory:** 64 KiB address-space abstraction.
- **BIOS:** host-independent cold/warm boot and console abstraction.
- **BDOS:** initial console services with a clean service-dispatch API.
- **Directory:** CP/M-style user area and 8.3 filename constraints.
- **Storage:** virtual sector storage and storage-observer events.
- **Audio:** event-driven retro audio and WAV output.
- **GUI:** backend-neutral GUI plus optional SDL3 adapter.
- **CLI:** `cpm22` development executable.

See [`docs/CPM22_ARCHITECTURE.md`](docs/CPM22_ARCHITECTURE.md) for the subsystem model and compatibility roadmap.

## Build

Requirements: CMake 3.20+ and a C++23 compiler.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

Run the development shell:

```bash
./build/cpm22
```

Optional SDL3 frontend:

```bash
cmake -S . -B build -DCPM22_ENABLE_SDL3=ON
cmake --build build --parallel
```

## Compatibility roadmap

The next major layers are FCB/file services, disk parameter blocks, deblocking, CCP command parsing, transient-program loading, boot-image generation, and expanded conformance tests. The repository currently provides the foundation for those layers rather than claiming full CP/M application compatibility.

## Project identity

See [`PROJECT_NAME.md`](PROJECT_NAME.md) for product and version metadata.
