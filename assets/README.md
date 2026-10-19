# Assets

Assets for the CP/M Universal Development Environment are organized by runtime purpose and kept separate from source code.

## Layout

```text
assets/
├── audio/          # Retro storage/event audio, tones, profiles
├── branding/       # Project logos, icons, product marks
├── desktop/        # Desktop backgrounds, shell resources
├── fonts/          # Bundled fonts only when licensing permits
├── icons/          # UI and application icons
├── images/         # Documentation and runtime images
├── maps/           # UML/tree/layout source data
├── themes/         # Desktop/XMB themes and palettes
├── ui/              # UI textures, cursors, widgets
├── cpm/             # CP/M guest media and reference assets
├── firmware/        # Firmware test images and manifests
├── disk-images/     # Virtual disk fixtures and test images
├── test-data/       # Regression fixtures
└── generated/      # Generated assets; never treated as source
```

Binary assets should include a nearby `.md` metadata file describing origin, license, format, dimensions, checksum, and intended use. Third-party assets require an entry in `THIRD_PARTY.md`.

Do not commit build outputs into this directory.
