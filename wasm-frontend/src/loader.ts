import os from "../../os.wasm?url";
import { imports, setWasmInstance } from "./imports";

const VGA_BUFFER_OFFSET = 0xb8000 as const;
const VGA_BUFFER_SIZE = 4000 as const;

export async function loadOS() {
  const response = await fetch(os);
  const { instance } = await WebAssembly.instantiateStreaming(response, {
    env: { ...imports.env },
  });

  setWasmInstance(instance);

  const memory = instance.exports.memory as WebAssembly.Memory;
  const requiredBytes = VGA_BUFFER_OFFSET + VGA_BUFFER_SIZE;
  const currentBytes = memory.buffer.byteLength;

  if (currentBytes < requiredBytes) {
    const pagesNeeded = Math.ceil((requiredBytes - currentBytes) / 65536);
    memory.grow(pagesNeeded);
  }

  return instance;
}
