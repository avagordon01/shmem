#!/usr/bin/env bash

set -eu

function cleanup {
    set +e
    {
        kill $(jobs -p)
        rm -rf /dev/shm/*
    } 2>/dev/null
}
trap cleanup EXIT

ninja -C build

./build/shmem_test shmem &
./build/shmem_test shmem_view &
wait
