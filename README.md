# CP/M 2.2 Clone

Clean-room CP/M 2.2-style emulator architecture with a portable C++23 core, Z80 target, CCP/BDOS/BIOS layers, virtual storage, CMOS/RTC hardware, optional Linux guest integration, and event-driven retro audio.

## Portable targets

The core is designed to build on Linux, Windows, macOS, and Android. Android is packaged as an APK through a thin JNI/native layer so the emulator core remains platform independent.

See `docs/portable-targets.md` for the APK architecture and size strategy.

## Boot

`boot/cpm_bootloader.asm` contains the original CP/M-style boot protocol used by the emulator. It selects and reads boot media through the emulator BIOS, reports failures, and transfers control to the CCP load address.

## CMOS/RTC

`src/platform/cmos_rtc.*` provides a small CMOS/RTC register model suitable for emulation and persistence integration.

## Linux option

`src/platform/linux_kernel_layer.*` provides the integration boundary for an optional Linux guest. The repository does not falsely claim that a compatibility layer is the complete Linux kernel. A specific upstream Linux release can be added as a separately versioned/submodule guest source tree and boot image.

## Storage audio

CP/M storage operations drive the audio layer:

```text
CP/M operation -> BIOS/BDOS -> virtual disk -> StorageAudioBridge -> RetroAudioEngine
                                                        |
                                      retro carrier + Morse timing
```

Read, write, save, load, create, seek, printer, and error events can produce adjustable-frequency/noise retro audio with Morse-style dot/dash timing.

## Build

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

The implementation is an independent compatibility project and does not reproduce historical CP/M source code.
