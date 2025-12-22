#!/usr/bin/env bash
set -e

for src in $(find src -type f -name '*.c'); do
    obj="${src%.c}.o"
    clang -std=gnu99 --target=wasm32 -O3 -flto -nostdlib \
      -I src/libc -D__wasm__ -D__wasm__serial__ \
      -c "$src" -o "$obj"
done

clang -std=gnu99 --target=wasm32 -O3 -flto -nostdlib \
  -Wl,--no-entry \
  -Wl,--export-all \
  -Wl,--lto-O3 \
  -Wl,--allow-undefined \
  $(find src -type f -name '*.o') \
  -o os.wasm \
  -v
