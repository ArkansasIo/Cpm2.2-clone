#!/usr/bin/env bash
set -euo pipefail

LINUX_VERSION="${LINUX_VERSION:-v6.12.60}"
ROOT="$(cd "$(dirname "$0")/.." && pwd)"
DEST="$ROOT/third_party/linux"

if [[ -d "$DEST/.git" ]]; then
  git -C "$DEST" fetch --tags --depth 1 origin "$LINUX_VERSION"
  git -C "$DEST" checkout --detach "$LINUX_VERSION"
else
  rm -rf "$DEST"
  git clone --depth 1 --branch "$LINUX_VERSION" https://git.kernel.org/pub/scm/linux/kernel/git/torvalds/linux.git "$DEST"
fi

echo "Linux kernel source checked out at $LINUX_VERSION"
git -C "$DEST" rev-parse HEAD
