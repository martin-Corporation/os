import {
  VGA_COLOR_TO_COLOR,
  COLUMNS,
  ROWS,
  CHAR_WIDTH,
  CHAR_HEIGHT,
} from "./constants";
import { getCursorState } from "./cursor";

const canvas = document.querySelector("canvas")!;
const ctx = canvas.getContext("2d")!;

function resizeCanvas() {
  const logicalWidth = window.innerWidth;
  const logicalHeight = window.innerHeight;

  canvas.width = logicalWidth;
  canvas.height = logicalHeight;
  canvas.style.width = logicalWidth + "px";
  canvas.style.height = logicalHeight + "px";
}

resizeCanvas();

window.addEventListener("resize", resizeCanvas);

function getCellSize() {
  const w = window.innerWidth;
  const h = window.innerHeight;

  const perCol = w / COLUMNS;
  const perRow = h / ROWS;

  const scaleW = perCol / CHAR_WIDTH;
  const scaleH = perRow / CHAR_HEIGHT;

  const cellWidth = scaleW * CHAR_WIDTH;
  const cellHeight = scaleH * CHAR_HEIGHT;

  return {
    width: cellWidth,
    height: cellHeight,
    fontSize: cellHeight,
  };
}

export function renderTerminal(buffer: Uint16Array) {
  const { width: cellW, height: cellH, fontSize } = getCellSize();

  ctx.font = `${fontSize}px "mx_univga", "px_univga", "MorePerfectDOSVGA", monospace`;
  ctx.textAlign = "center";
  ctx.textBaseline = "middle";

  ctx.fillStyle = "#000000";
  ctx.fillRect(0, 0, canvas.width, canvas.height);

  for (let i = 0; i < buffer.length; i++) {
    const entry = buffer[i];
    const charCode = entry & 0xff;
    const attr = (entry >> 8) & 0xff;
    const fg = attr & 0x0f;
    const bg = (attr >> 4) & 0x0f;

    if (charCode === 0 && fg === 0 && bg === 0) continue;

    const col = i % COLUMNS;
    const row = Math.floor(i / COLUMNS);

    const x = col * cellW;
    const y = row * cellH;

    ctx.fillStyle = VGA_COLOR_TO_COLOR[bg];
    ctx.fillRect(x, y, cellW, cellH);

    if (charCode >= 32) {
      ctx.fillStyle = VGA_COLOR_TO_COLOR[fg];
      ctx.fillText(String.fromCharCode(charCode), x + cellW / 2, y + cellH / 2);
    }
  }
}

export function renderCursor(cursorVisible: boolean, buffer: Uint16Array) {
  const { width: cellW, height: cellH } = getCellSize();
  const { start, low, high } = getCursorState();
  const pos = (high << 8) | low;
  const cursorRow = Math.floor(pos / COLUMNS);
  const cursorCol = pos % COLUMNS;
  const cursorEnabled = (start & 0x20) === 0;

  try {
    if (cursorEnabled && cursorVisible) {
      const cx = cursorCol * cellW;
      const cy = cursorRow * cellH;

      ctx.fillStyle = "#a1a0a0";
      ctx.fillRect(
        cx,
        cy + (cellH + cellH / 2) / 2,
        cellW,
        (cellH - cellH / 2) / 2 / 2,
      );
    } else if (cursorEnabled && !cursorVisible) {
      const cx = cursorCol * cellW;
      const cy = cursorRow * cellH;

      let i = -1;
      let col;
      let row;

      while (col != cursorCol && row != cursorRow) {
        col = i % COLUMNS;
        row = Math.floor(i / COLUMNS);
        i++;
      }

      const entry = buffer[i];
      const attr = (entry >> 8) & 0xff;
      const bg = (attr >> 4) & 0x07;

      ctx.fillStyle = VGA_COLOR_TO_COLOR[bg];
      ctx.fillRect(
        cx,
        cy + (cellH - cellH / 2) / 2 / 2,
        cellW,
        (cellH - cellH / 2) / 2 / 2,
      );
    }
  } catch (error) {
    console.error(error);
  }
}
