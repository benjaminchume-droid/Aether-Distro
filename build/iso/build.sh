#!/bin/sh
set -eu
ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
OUT="$ROOT/build/aether.iso"
STAGE="$ROOT/build/iso-root"
KERNEL="$ROOT/build/aether-kernel"
INITRD="$ROOT/build/initramfs/aether-initramfs.cpio"
command -v grub-mkrescue >/dev/null 2>&1 || { echo "grub-mkrescue is required" >&2; exit 1; }
command -v xorriso >/dev/null 2>&1 || { echo "xorriso is required" >&2; exit 1; }
[ -f "$KERNEL" ] || { echo "kernel missing; run 'make kernel'" >&2; exit 1; }
[ -f "$INITRD" ] || { echo "initramfs missing; run 'make initramfs'" >&2; exit 1; }
rm -rf "$STAGE" "$OUT"
mkdir -p "$STAGE/boot/grub"
cp "$KERNEL" "$STAGE/boot/aether-kernel"
cp "$INITRD" "$STAGE/boot/aether-initramfs.cpio"
cp "$ROOT/boot/grub/grub.cfg" "$STAGE/boot/grub/grub.cfg"
grub-mkrescue -o "$OUT" "$STAGE" >/dev/null
echo "$OUT"
