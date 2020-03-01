#!/usr/bin/env bash

set -eu
trap 'kill $(jobs -p) 2>/dev/null' EXIT

ninja -C build

./build/shmem_test shmem &
./build/shmem_test shmem_view &
wait

rm -rf /dev/shm/aether_ro
