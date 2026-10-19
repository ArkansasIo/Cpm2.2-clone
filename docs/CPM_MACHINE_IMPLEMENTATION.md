# CP/M machine implementation roadmap

## Layers

1. **CPU** — Intel 8080 execution and cycle accounting.
2. **Memory** — 64 KiB address space.
3. **Machine** — CPU/memory binding and program loading.
4. **Console** — host-independent text output buffer.
5. **BIOS** — console, disk, list, and system entry points.
6. **BDOS** — CP/M system-call dispatcher.
7. **CCP** — command processor and transient program loader.
8. **Filesystem** — CP/M-compatible directory, allocation, and file semantics.
9. **Disk images** — sector/block/DPB support and persistence.
10. **Frontend** — terminal and optional SDL3 UI.

## Engineering rules

- Keep hardware and host-platform code behind interfaces.
- Keep CPU execution deterministic.
- Do not silently treat unsupported opcodes as successful execution.
- Add regression tests before expanding instruction coverage.
- Keep CP/M behavior separate from host filesystem behavior.
- Treat disk image parsing as untrusted input and validate bounds.

## Next implementation target

The next major component should be a BIOS/BDOS service layer connected to the virtual disk. That layer should be testable without a graphical frontend and should expose explicit CP/M-compatible return values.
