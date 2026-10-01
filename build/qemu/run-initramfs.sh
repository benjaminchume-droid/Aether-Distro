#!/bin/sh
set -eu
ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
KERNEL="${1:-$ROOT/build/aether-kernel}"
INITRD="$ROOT/build/initramfs/aether-initramfs.cpio"
[ -f "$KERNEL" ] || { echo "kernel not found: $KERNEL; run 'make kernel' first" >&2; exit 1; }
[ -f "$INITRD" ] || { echo "initramfs not found; run 'make initramfs' first" >&2; exit 1; }
exec qemu-system-x86_64 -machine q35 -m 2048 -smp 2 -nographic -no-reboot  -kernel "$KERNEL" -initrd "$INITRD"  -append "console=ttyS0 rdinit=/init panic=-1"
