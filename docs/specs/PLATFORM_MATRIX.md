# Platform Matrix

| Target | Primary build | Runtime role |
|---|---|---|
| Linux x86_64 | C++23/CMake | Native desktop host |
| Raspberry Pi 3 | ARM64 Linux | Native lightweight desktop host |
| Raspberry Pi 4 | ARM64 Linux | Native desktop host |
| Raspberry Pi 5 | ARM64 Linux | Native desktop host |
| Windows | C++23/CMake | Native desktop host |
| macOS | C++23/CMake | Native desktop host |
| Android | NDK/C++23 + app shell | Native application inside Android |
| iOS | Xcode/Clang + C++23 core | Native application inside iOS |

The CP/M guest is platform-independent and runs inside the host runtime. A Raspberry Pi package is an ARM64 Linux application or image, depending on the release profile; the Android/iOS packages remain applications and do not replace the device operating system.

## Hardware constraints
Pi 3 receives the conservative feature baseline: lightweight rendering, bounded memory use, no Pi-5-specific APIs, and optional accelerated graphics. P20/P40 mobile builds prioritize touch, lifecycle suspension/resume, and storage permissions.
