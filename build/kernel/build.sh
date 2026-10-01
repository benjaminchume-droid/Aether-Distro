#!/bin/sh
set -eu
ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
VERSION="${AETHER_KERNEL_VERSION:-6.12.43}"
SRC="$ROOT/build/linux-$VERSION"
TARBALL="$ROOT/build/linux-$VERSION.tar.xz"
URL="https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-$VERSION.tar.xz"
mkdir -p "$ROOT/build"
if [ ! -d "$SRC" ]; then
  if [ ! -f "$TARBALL" ]; then
    command -v curl >/dev/null 2>&1 || { echo "curl is required" >&2; exit 1; }
    curl -fL "$URL" -o "$TARBALL"
  fi
  tar -xf "$TARBALL" -C "$ROOT/build"
fi
cp "$ROOT/02-kernel/aether-x86_64.config" "$SRC/build/.config"
make -C "$SRC" O="$SRC/build" ARCH=x86_64 olddefconfig
make -C "$SRC" O="$SRC/build" ARCH=x86_64 -j"$(getconf _NPROCESSORS_ONLN 2>/dev/null || echo 2)"
cp "$SRC/build/arch/x86/boot/bzImage" "$ROOT/build/aether-kernel"
echo "$ROOT/build/aether-kernel"
