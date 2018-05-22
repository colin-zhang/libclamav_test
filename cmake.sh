#!/bin/sh

set -e
set -u

if [[ ! -d  build ]]; then
    mkdir -p build
fi

cd build && cmake .. && make