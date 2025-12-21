import "./style.css";
import os from "../../os.wasm?url";

function decodeVgaColor(vgaByte: number) {
  const byte = vgaByte & 0xff;
  const fg = byte & 0x0f;
  const bg = (byte >> 4) & 0x0f;

  return { fg, bg };
}

let wasmInstance: WebAssembly.Instance;
const decoder = new TextDecoder();

const vgaColorToCSS = [
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
    panic: (ptr: number) => {
      if (!wasmInstance) return;

      const memory = wasmInstance.exports.memory as WebAssembly.Memory;
      const bytes = new Uint8Array(memory.buffer);

      let end = ptr;
      while (bytes[end] !== 0) {
        end++;
      }

      const text = decoder.decode(bytes.subarray(ptr, end));
      console.error(text);
      document.body.append(
        Object.assign(document.createElement("span"), {
          innerText: "[panic] " + text,
          style: "white-space: pre; font-family: monospace",
        }),
      );
    },

    js_console_log_args: (ptr: number, len: number, useConsole: number) => {
      if (!wasmInstance) return;

      const memory = wasmInstance.exports.memory as WebAssembly.Memory;
      const bytes = new Uint8Array(memory.buffer, ptr, len);

      const text = decoder.decode(bytes);

      if (useConsole) {
        console.log(text);
      } else {
        const color = (
          wasmInstance.exports.get_terminal_color as any as () => number
        )();
        const fgAndBg = decodeVgaColor(color);

        document.body.append(
          Object.assign(document.createElement("span"), {
            innerText: text,
            style: `white-space: pre; color: ${vgaColorToCSS[fgAndBg.fg]}; background-color: ${vgaColorToCSS[fgAndBg.bg]};`,
          }),
        );
      }
    },
    enable_interrupts: () => {},
    asm_panic() {
      if (!wasmInstance) return;
      this.panic(-1);
    },
    puts(ptr: number) {
      if (!wasmInstance) return;
      const memory = wasmInstance.exports.memory as WebAssembly.Memory;
      const bytes = new Uint8Array(memory.buffer);

      let end = ptr;
      while (bytes[end] !== 0) {
        end++;
      }

      const text = decoder.decode(bytes.subarray(ptr, end));
      console.log(text);
      document.body.append(
        Object.assign(document.createElement("span"), {
          innerText: text,
          style:
            "white-space: pre; font-family: monospace; color: white !important",
        }),
      );
    },
  },
};

for (let i = 0; i < 256; i++) {
  imports.env[`i686_ISR${i}` as keyof typeof imports.env] = console.log;
}

WebAssembly.instantiateStreaming(fetch(os), imports)
  .then(({ instance }) => {
    wasmInstance = instance;
    console.log(instance);

    if (instance.exports.kmain) {
      (instance.exports.kmain as Function)();
    } else {
      console.warn("kmain not found");
    }
  })
  .catch((...err) => {
    console.error(...(Array.isArray(err) ? err : err ? [err] : []));
    document.body.innerHTML +=
      "<p>Couldn't load OS. See console for details.</p>";
  });
