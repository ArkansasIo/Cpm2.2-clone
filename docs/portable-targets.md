# Portable targets

The CP/M emulator is designed as a portable core rather than an APK-only application.

## Target environments

- Linux x86_64/ARM64
- Windows x86_64/ARM64
- macOS x86_64/ARM64
- Android APK (ARM64 and, where supported by the toolchain, x86_64)
- Embedded/minimal environments using the C++ core without the optional host UI

## Android packaging

The recommended APK architecture is:

```text
Android Activity / JNI
        |
        v
  cpm22_core (C++23)
   |     |      |
  Z80   CP/M   CMOS
        BIOS    RTC
          |
      storage audio
          |
       WAV/AudioTrack
```

Keep the emulator core free of Android-specific APIs. Put JNI, APK resources, permissions, and the Android UI in a separate `android/` target. This allows the same source tree to build as a desktop executable and as an APK.

## Size strategy

The core should remain small by default: no bundled desktop assets, no mandatory Linux kernel image, and no large prerecorded audio file. Generate test audio on demand and package a Linux kernel as an optional guest artifact.
