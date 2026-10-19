# CP/M 2.2 Clone

Clean-room compatible CP/M 2.2-style emulator architecture with Z80 CPU, CCP/BDOS/BIOS layers, disk I/O, and live 8-bit storage audio integration.

## Test scope

The test suite is designed to verify the complete event chain:

CP/M operation -> BIOS/BDOS disk I/O -> storage callback -> 8-bit audio event -> printer/Pong sound layer.

The repository also includes the previously generated 60-second retro storage/Pong/printer WAV as a local test asset when added to the project workspace.

## Build

```bash
cmake -S . -B build
cmake --build build
ctest --test-dir build --output-on-failure
```

## Design

The implementation uses clean-room compatibility goals and does not reproduce proprietary CP/M source code. Storage audio is event-driven from real storage operations rather than a fixed prerecorded activity timer.
