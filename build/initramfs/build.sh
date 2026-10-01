#!/bin/sh
set -eu
ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
OUT="$ROOT/build/initramfs"
STAGE="$OUT/root"
mkdir -p "$STAGE"/{bin,sbin,etc,proc,sys,dev,run,tmp}
if [ ! -x "$ROOT/build/aether-init" ]; then
  echo "build/aether-init is missing; run 'make aether-init' first" >&2
  exit 1
fi
cp "$ROOT/build/aether-init" "$STAGE/init"
chmod 0755 "$STAGE/init"
rm -f "$OUT/aether-initramfs.cpio"
(
  cd "$STAGE"
  find . -print | cpio -o -H newc --quiet > "$OUT/aether-initramfs.cpio"
)
echo "$OUT/aether-initramfs.cpio"
