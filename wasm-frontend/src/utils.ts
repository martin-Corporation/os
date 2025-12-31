import { getWasmInstance } from "./imports";

function allocateString(
  str: string,
  alloc: (size: number) => number,
  memory: WebAssembly.Memory,
): number {
  const encoder = new TextEncoder();
  const bytes = encoder.encode(str);
  const ptr = alloc(bytes.length + 1);
  const buf = new Uint8Array(memory.buffer, ptr, bytes.length + 1);

  buf.set(bytes);
  buf[bytes.length] = 0;

  return ptr;
}

export function readString(
  pointer: number,
  memory: WebAssembly.Memory,
): string {
  const bytes = new Uint8Array(memory.buffer);
  let end = pointer;

  while (bytes[end] !== 0) {
    end++;
  }

  const slice = bytes.subarray(pointer, end);
  const str = new TextDecoder("utf8").decode(slice);

  return str;
}

export function callExport(name: string, ...args: any[]) {
  const instance = getWasmInstance();

  if (!instance) return;

  const fn = (instance.exports as any)[name];

  if (typeof fn !== "function") {
    console.warn(`Export ${name} not found`);
    return;
  }

  const memory = instance.exports.memory as WebAssembly.Memory;
  const alloc = instance.exports.malloc as any;
  const free = instance.exports.free as any;

  if (typeof alloc !== "function" || typeof free !== "function") {
    throw new Error("Couldn't find malloc and free");
  }

  const pointers: number[] = [];
  const converted: number[] = [];

  for (let arg of args) {
    switch (typeof arg) {
      case "number":
        converted.push(arg);
        break;

      case "bigint": {
        const num = Number(arg);
        converted.push(num);
        break;
      }

      case "boolean":
        converted.push(arg ? 1 : 0);
        break;

      case "string": {
        const ptr = allocateString(arg, alloc, memory);
        pointers.push(ptr);
        converted.push(ptr);
        break;
      }

      default:
        console.warn(`unsupported arg type "${typeof arg}", using 0`);
        converted.push(0);
        break;
    }
  }

  const result = fn(...converted);

  for (const ptr of pointers) {
    free(ptr);
  }

  return result;
}
