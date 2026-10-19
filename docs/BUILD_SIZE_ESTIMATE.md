# Build Size Estimate

These are planning estimates, not measured release binaries.

| Target | Expected optimized application/package |
|---|---:|
| Linux desktop | 6–30 MB |
| Windows | 10–40 MB |
| macOS | 10–40 MB |
| Android ARM64 APK | 10–35 MB |
| iOS ARM64 | 10–40 MB |
| Raspberry Pi | 6–30 MB |

Debug builds may be 2–10x larger because of symbols and diagnostics. Bundled runtime assets, SDK toolchains, fonts, sample disk images, and third-party libraries are additional.

The complete source/documentation repository is expected to be approximately 50–200 MB initially, with substantial variation based on binary fixtures and artwork.

## Size controls

- Build Release configurations for distribution.
- Strip symbols from release executables; publish symbols separately where appropriate.
- Keep SDK toolchains separate from the runtime package.
- Keep test fixtures out of production packages.
- Compress documentation and optional assets.
- Package platform-specific resources only for the target platform.
- Track package size in release CI.

Actual sizes must replace these estimates once reproducible builds exist for each target.
