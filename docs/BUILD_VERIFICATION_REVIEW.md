# Build and Verification Status

## Current repository snapshot

The repository currently contains source files for storage, audio, CMOS/RTC, architecture metadata, GUI adapters, and a Linux-kernel integration boundary. It also includes one integration-style test source, `tests/test_cpm_audio.cpp`.

## Important build-system finding

The inspected `main` tree does not contain a root `CMakeLists.txt` or any other recognized build configuration file. As a result, the documented CMake commands in the README cannot currently be run from this repository snapshot.

## Test-source issues found during inspection

The current storage API accepts `std::vector<std::uint8_t>` for reads and writes, while the test passes `std::array<std::uint8_t, 128>`. Those calls do not match the declared API and should be corrected before treating the test as buildable. The test also uses `std::array` without directly including `<array>`.

## Recommended next verification steps

1. Add a root CMake build that explicitly lists source files, declares C++23, and registers the integration test with CTest.
2. Correct the test fixture to use the storage API's vector-based buffers (or deliberately expand the API to span-based buffers).
3. Add focused tests for storage boundary handling, invalid geometry, CMOS register behavior, and Linux guest option validation.
4. Add a CI workflow that configures, builds, and runs CTest on a clean Ubuntu runner.

## Accuracy note

This document records a source inspection only. No compilation or runtime tests are claimed to have passed.
