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

export const COLUMNS = 80;
export const ROWS = 25;
export const BUFFER_SIZE = COLUMNS * ROWS;
export const CHAR_WIDTH = 8;
export const CHAR_HEIGHT = 16;
