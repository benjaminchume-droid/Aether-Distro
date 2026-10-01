#!/bin/sh
set -eu
ROOT="$(CDPATH= cd -- "$(dirname -- "$0")/../.." && pwd)"
ISO="${1:-$ROOT/build/aether.iso}"
[ -f "$ISO" ] || { echo "ISO not found: $ISO; run 'make iso' first" >&2; exit 1; }
exec qemu-system-x86_64 -machine q35 -m 2048 -smp 2 -nographic -cdrom "$ISO" -no-reboot
