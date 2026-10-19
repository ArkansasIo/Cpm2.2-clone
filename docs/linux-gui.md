# Linux GUI

The project now has a C++ GUI layer separate from the 8-bit guest OS.

The GUI is a **64-bit host-side application layer**. It does not change the guest architecture: the emulated machine remains an 8-bit Z80/CP/M-style environment.

Recommended backend options are SDL2 for a lightweight portable frontend, with Wayland/X11 supplied by the Linux platform through SDL. The same frontend architecture can later be reused by the Android APK with a platform-specific window/audio backend.

```text
Linux 64-bit desktop
        |
    C++ GUI layer
        |
  emulator frontend
        |
  8-bit CP/M guest
        |
 Z80 + BIOS + BDOS + CCP
```

The initial `LinuxGui` implementation provides the lifecycle boundary without forcing a heavyweight GUI dependency into the core library.
