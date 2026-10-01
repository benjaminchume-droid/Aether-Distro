#!/bin/sh
set -eu
ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
KERNEL="$1"
[ -f "$KERNEL" ] || { echo "kernel not found: $KERNEL" >&2; exit 1; }
exec qemu-system-x86_64 -machine q35 -m 2048 -smp 2 -nographic -no-reboot   -kernel "$KERNEL" -initrd "$ROOT/build/initramfs/aether-initramfs.cpio"   -append "console=ttyS0 rdinit=/init"
