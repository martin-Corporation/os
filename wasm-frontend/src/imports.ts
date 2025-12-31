import { ps2_keyboard_initialize } from "./keyboard";
import {
  disableCursor,
  enableCursor,
  getCursorPosition,
  updateCursor,
} from "./terminal/cursor";
import { readString } from "./utils";

let _wasmInstance: WebAssembly.Instance | undefined;
export let interruptsEnabled = true;

export const VGA_COLOR_TO_COLOR = [
  "black", // VGA_COLOR_BLACK = 0,
  "blue", // VGA_COLOR_BLUE = 1,
  "#00a500", // VGA_COLOR_GREEN = 2,
  "cyan", // VGA_COLOR_CYAN = 3,
  "red", // VGA_COLOR_RED = 4,
  "purple", // VGA_COLOR_MAGENTA = 5,
  "brown", // VGA_COLOR_BROWN = 6,
  "#a1a0a0", // VGA_COLOR_LIGHT_GREY = 7,
  "darkgrey", // VGA_COLOR_DARK_GREY = 8,
  "lightblue", // VGA_COLOR_LIGHT_BLUE = 9,
  "lightgreen", // VGA_COLOR_LIGHT_GREEN = 10,
  "lightcyan", // VGA_COLOR_LIGHT_CYAN = 11,
  "lightred", // VGA_COLOR_LIGHT_RED = 12,
  "magenta", // VGA_COLOR_LIGHT_MAGENTA = 13,
  "yellow", // VGA_COLOR_LIGHT_BROWN = 14,
  "white", // VGA_COLOR_WHITE = 15,
] as const;

const imports = {
  env: {
    enable_cursor: enableCursor,
    disable_cursor: disableCursor,
    update_cursor: updateCursor,
    get_cursor_position: getCursorPosition,
    ps2_keyboard_initialize,
    asm: (pointer: number) => {
      if (!getWasmInstance()) return;

      const instruction = readString(
        pointer,
        getWasmInstance()!.exports.memory as WebAssembly.Memory,
      );

      switch (instruction) {
        case "cli": {
          interruptsEnabled = false;
          break;
        }

        case "sti": {
          interruptsEnabled = true;
          break;
        }

        default: {
          console.warn("Unknown ASM instruction:", instruction);
        }
      }
    },
  } as Record<string, Function>,
};

for (let i = 0; i < 256; i++) {
  imports.env[`i686_ISR${i}` as keyof typeof imports.env] = console.log;
}

export function setWasmInstance(instance: WebAssembly.Instance) {
  _wasmInstance = instance;
}

export function getWasmInstance(): WebAssembly.Instance | undefined {
  return _wasmInstance;
}

export { imports };
