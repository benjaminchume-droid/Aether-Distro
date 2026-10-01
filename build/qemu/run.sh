#!/bin/sh
set -eu
IMAGE="$1"
[ -f "$IMAGE" ] || { echo "Aether image not found: $IMAGE" >&2; exit 1; }
exec qemu-system-x86_64 -machine q35 -m 2048 -smp 2 -serial stdio -drive format=raw,file="$IMAGE"
