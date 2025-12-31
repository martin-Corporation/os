import { renderCursor, renderTerminal } from "./renderer";
import { BUFFER_SIZE } from "./constants";

const VGA_BUFFER_OFFSET = 0xb8000;

let terminalBuffer: Uint16Array;

export function startRenderer(instance: WebAssembly.Instance) {
  const memory = instance.exports.memory as WebAssembly.Memory;
  const canvas = document.querySelector("canvas")!;
  const ctx = canvas.getContext("2d")!;

  terminalBuffer = new Uint16Array(
    memory.buffer,
    VGA_BUFFER_OFFSET,
    BUFFER_SIZE,
  );

  let cursorVisible = true;

  setInterval(() => {
    cursorVisible = !cursorVisible;
  }, 500);

  function render() {
    renderTerminal(terminalBuffer);
    renderCursor(cursorVisible, terminalBuffer);
    requestAnimationFrame(render);
  }

  render();

  window.addEventListener("resize", () => {
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    renderTerminal(terminalBuffer);
  });
}
