# Linux kernel source integration

This directory reserves the exact location for a pinned Linux kernel source checkout.

The repository does not duplicate the multi-million-line upstream kernel through generated API commits. Instead, `scripts/fetch-linux-kernel.sh` downloads a selected release into this directory so the complete upstream source is present locally and can be tracked as a submodule or source snapshot according to the project's distribution policy.

## Recommended reproducible source

Set `LINUX_VERSION` to the exact kernel release you want, for example `v6.12.XX`, then run the fetch script. Record the resulting Git commit/tag in the build metadata.

The CP/M emulator itself remains independent of Linux and can boot the CP/M-style loader without a Linux guest.
