# CP/M Subsystem Specification

## Boot
The boot pipeline loads a documented boot image, initializes guest memory and devices, and transfers control to the CP/M entry point.

## BIOS
The BIOS boundary owns machine-dependent console, disk, list, and system services. The host implementation must keep BIOS semantics separate from host filesystem APIs.

## BDOS
BDOS exposes CP/M operating-system services through the guest calling convention. Calls are decoded, validated, dispatched, and returned with deterministic result codes.

## CCP
The command processor parses console commands, resolves transient/internal commands, and dispatches executable programs through the BDOS interface.

## Disk and filesystem
Virtual drives use a documented geometry and sector abstraction. Directory entries, allocation state, timestamps where supported, and file contents are serialized in a portable image format.

## Compatibility rule
A feature is labelled CP/M-compatible only after behaviour is covered by executable tests or an explicitly documented compatibility limitation.
