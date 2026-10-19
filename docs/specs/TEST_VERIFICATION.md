# Test and Verification Plan

## Unit layers
CPU instructions, flags, memory, virtual disk sectors, filesystem allocation, BIOS calls, BDOS dispatch, command parsing, device events, audio event encoding, version metadata.

## Integration layers
Boot-to-CCP, program load/execute, terminal I/O, disk mount/read/write, debugger breakpoints, SDK build/run/debug, desktop services, XMB navigation, update/rollback metadata.

## Cross-platform matrix
Each release candidate is built for Linux x86_64, Raspberry Pi ARM64, Windows, macOS, Android, and iOS when toolchains are available. Guest-level tests must remain host-independent.

## Determinism
Replay the same event stream against the same initial guest state and verify identical CPU state, memory state, disk changes, and generated audio event sequence.

## Release gates
No release is marked stable until documentation, build metadata, automated tests, packaging checks, and known limitations are recorded.
