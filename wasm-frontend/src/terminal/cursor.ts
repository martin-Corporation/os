import { COLUMNS } from "./constants";

let cursorStart = 0;
let cursorEnd = 0;
let cursorLow = 0;
let cursorHigh = 0;

export function getCursorState() {
  return {
    start: cursorStart,
    end: cursorEnd,
    low: cursorLow,
    high: cursorHigh,
  };
}

export function enableCursor(cursorStart: number, cursorEnd: number) {
  cursorStart = cursorStart & ~0x20;
  cursorEnd = cursorEnd;
}

export function disableCursor() {
  cursorStart |= 0x20;
}

export function updateCursor(x: number, y: number) {
  const pos = (y * COLUMNS + x) & 0xffff;
  cursorLow = pos & 0xff;
  cursorHigh = (pos >> 8) & 0xff;
}

export function getCursorPosition() {
  return (cursorHigh << 8) | cursorLow;
}
