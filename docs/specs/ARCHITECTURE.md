# Architecture Specification

## Purpose
Define the stable boundary between the 64-bit host environment and the 8-bit CP/M guest.

## Layers
1. Host runtime: process, filesystem sandbox, threads, clocks, diagnostics.
2. Platform adapter: Linux, Windows, macOS, Android, iOS, Raspberry Pi.
3. Desktop shell: windows, panels, dock, taskbar, XMB navigation, settings.
4. Emulation core: CPU, memory, I/O bus, virtual disks, CP/M BIOS/BDOS/CCP.
5. SDK/tooling: assembler, C/C++ toolchain adapters, debugger, project manager.
6. Applications: terminal, file manager, editor, calculator, help, diagnostics.

## Invariants
- Guest address space is 64 KiB for the baseline 8080/Z80 model.
- Guest state is deterministic for identical input, device state, and virtual media.
- Host-specific APIs are isolated behind platform interfaces.
- UI state never directly mutates CPU state; commands cross an application/service boundary.

## Data flow
Input -> command/action -> application service -> guest/device interface -> deterministic state transition -> event stream -> UI/audio/telemetry.

## Status
Design specification; implementation must be verified by tests before being labelled complete.
