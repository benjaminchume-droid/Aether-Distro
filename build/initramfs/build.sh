#!/bin/sh
set -eu
ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
OUT="$ROOT/build/initramfs"
STAGE="$OUT/root"
rm -rf "$STAGE" "$OUT/aether-initramfs.cpio"
mkdir -p "$STAGE"/{bin,sbin,etc,proc,sys,dev,run,tmp}
[ -x "$ROOT/build/aether-init" ] || { echo "build/aether-init is missing; run 'make aether-init' first" >&2; exit 1; }
cp "$ROOT/build/aether-init" "$STAGE/init"
chmod 0755 "$STAGE/init"
if command -v busybox >/dev/null 2>&1; then
  cp "$(command -v busybox)" "$STAGE/bin/busybox"
  chmod 0755 "$STAGE/bin/busybox"
  ln -sf busybox "$STAGE/bin/sh"
  ln -sf busybox "$STAGE/bin/ls"
  ln -sf busybox "$STAGE/bin/cat"
  ln -sf busybox "$STAGE/bin/mount"
  ln -sf busybox "$STAGE/bin/echo"
  ln -sf busybox "$STAGE/bin/dmesg"
  echo "busybox included: emergency shell available"
else
  echo "warning: busybox not found; boot remains headless (no shell)"
fi
(
 cd "$STAGE"
 find . -print | cpio -o -H newc --quiet > "$OUT/aether-initramfs.cpio"
)
echo "$OUT/aether-initramfs.cpio"
