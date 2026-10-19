# Technical References and Research Policy

## Research scope

The project may study public technical documentation, standards, open-source implementations, emulator behavior, and historical interface descriptions to establish compatible behavior.

## Clean-room implementation policy

Behavior and interfaces may be independently reimplemented. Source code is not copied unless its license explicitly permits the intended use and all required notices/conditions are retained. Proprietary or incompatible code is excluded.

## Reference categories

### Guest CPU
- Zilog Z80 programmer/reference documentation.
- Intel 8080 programmer/reference documentation.

### CP/M compatibility
- Publicly documented CP/M-compatible CCP, BDOS, BIOS, FCB, filesystem, and boot interfaces.
- Public historical technical references describing disk formats, allocation, directory records, and system calls.

### Host/runtime
- ISO C++23 language and standard-library specifications.
- CMake documentation.
- POSIX/Linux documentation.

### Platforms
- Android NDK/SDK documentation.
- Apple developer documentation for iOS/macOS.
- Microsoft Windows SDK documentation.
- Raspberry Pi hardware and Raspberry Pi OS/Linux documentation.

### UI and media
- SDL documentation where SDL is used.
- Public audio/file-format specifications for generated runtime media.

## Provenance requirements

Every external source that materially influences implementation should record:

- Source title
- Author/organization
- URL or publication identifier
- Access date
- Relevant version/revision
- License/copyright status
- Components influenced

For redistributed source or binary assets, also record the exact license and required attribution in `THIRD_PARTY.md`.
