# CP/M 2.2 Clone — CI Failure Review

Reviewed on 2026-10-19.

## Current finding

The latest GitHub Actions run for pull request #2 successfully configures and compiles the C++ project. Six of seven tests pass. The failing test is `cpm_filesystem`, at the rename regression assertion:

```cpp
assert(!fs.open(fcb));
```

The filesystem rename implementation stores the new filename including its extension in `CpmDirectoryRecord::name` (for example, `WORLD.TXT`) while the on-disk CP/M directory format stores the 8-character name and 3-character extension in separate fields. This causes the renamed directory entry to be encoded incorrectly and the old name lookup may continue to succeed.

## Recommended fix

When renaming, split `new_fcb.filename()` at the first period and assign the base name and extension independently. Preserve extent, allocation, and record count fields. Add regression coverage for rename across both plain and dotted names, then run the full CTest suite.

## Verification status

- CMake configure: passed
- C++ compilation: passed
- `cpm_audio_integration`: passed
- `cpu8080_regression`: passed
- `cpm_directory`: passed
- `cpm_bdos_services`: passed
- `cpm_fcb`: passed
- `cpm_filesystem`: failed (rename regression)
