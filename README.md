# CP/M Universal Development Environment

A clean-room C++23 project exploring a CP/M 2.2-style emulator and portable retro-computing environment. The current implementation is a scaffold of storage, audio, platform abstractions, and GUI adapter components; full CPU/CCP/BDOS/BIOS execution compatibility is not yet claimed.

## Project identity

See [PROJECT_NAME.md](PROJECT_NAME.md) for the product ID, development name, and version metadata.

## Current components

- Virtual sector storage and storage-observer events: `src/devices/`
- Event-driven retro audio and WAV output: `src/audio/`
- CMOS/RTC register model and host architecture metadata: `src/platform/`
- Backend-neutral GUI and optional SDL3 adapter: `src/gui/`
- Clean-room boot protocol sketch: `boot/`
- Architecture and subsystem specifications: `docs/specs/`

## Build and test

Prerequisites: CMake 3.20 or later and a C++23-capable compiler.

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

Optional SDL3 GUI adapter:

```bash
cmake -S . -B build -DCPM22_ENABLE_SDL3=ON
cmake --build build --parallel
```

SDL3 must be installed with a CMake package configuration discoverable by `find_package(SDL3 CONFIG REQUIRED)`.

GitHub Actions runs the same configure, build, and test sequence for pushes and pull requests. See [the workflow](.github/workflows/build.yml).

## Verification status

- The present CMake target compiles the source files listed in `CMakeLists.txt`.
- The storage/audio integration test is registered with CTest.
- CPU instruction execution, CCP, BDOS, BIOS, disk filesystem compatibility, and real boot-to-CCP operation remain future implementation/verification work.
- See [Build and Verification Review](docs/BUILD_VERIFICATION_REVIEW.md) for the known scope and verification boundaries.

## Clean-room and provenance policy

This project aims to implement behavior from public technical specifications and independently authored code. It is not the historical CP/M source tree and should not be described as a complete replacement until compatibility has been implemented and tested.
