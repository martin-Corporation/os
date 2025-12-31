import { interruptsEnabled } from "./imports";
import { callExport } from "./utils";

const KEYBOARD_US = [
  0,
  27,
  "1",
  "2",
  "3",
  "4",
  "5",
  "6",
  "7",
  "8",
  "9",
  "0",
  "-",
  "=",
  "\b",
  "\t",
  "q",
  "w",
  "e",
  "r",
  "t",
  "y",
  "u",
  "i",
  "o",
  "p",
  "[",
  "]",
  "\n",
  0,
  "a",
  "s",
  "d",
  "f",
  "g",
  "h",
  "j",
  "k",
  "l",
  ";",
  "'",
  "`",
  0,
  "\\",
  "z",
  "x",
  "c",
  "v",
  "b",
  "n",
  "m",
  ",",
  ".",
  "/",
  0,
  "*",
  0,
  " ",
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  "-",
  0,
  0,
  0,
  "=",
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
] as const;

const SHIFT_KEYBOARD_US = [
  0,
  27,
  "!",
  "@",
  "#",
  "$",
  "%",
  "^",
  "&",
  "*",
  "(",
  ")",
  "_",
  "+",
  "\b",
  "\t",
  "Q",
  "W",
  "E",
  "R",
  "T",
  "Y",
  "U",
  "I",
  "O",
  "P",
  "{",
  "}",
  "\n",
  0,
  "A",
  "S",
  "D",
  "F",
  "G",
  "H",
  "J",
  "K",
  "L",
  ":",
  '"',
  "~",
  0,
  "|",
  "Z",
  "X",
  "C",
  "V",
  "B",
  "N",
  "M",
  "<",
  ">",
  "?",
  0,
  "*",
  0,
  " ",
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  "_",
  0,
  0,
  0,
  "+",
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
] as const;

const KEY_TO_CHAR: Record<string, string> = {
  Enter: "\n",
} as const;

const KEY_TO_SCANCODE: Record<string, number> = {
  Backspace: 0x0e,
  Shift: 0x2a,
} as const;

export function ps2_keyboard_initialize() {
  window.addEventListener("keydown", (event) => {
    if (!interruptsEnabled) return;

    const layout = event.shiftKey ? SHIFT_KEYBOARD_US : KEYBOARD_US;
    const key = event.key in KEY_TO_CHAR ? KEY_TO_CHAR[event.key] : event.key;

    let scancode = layout.findIndex((e) =>
      typeof e === "string" ? e === key : false,
    );

    if (scancode === -1) {
      scancode = KEY_TO_SCANCODE[event.key] ?? 0;
    }

    callExport("queue_inb_data", 0x60, scancode);
    callExport("ps2_keyboard_handler");
  });

  window.addEventListener("keyup", (event) => {
    if (!interruptsEnabled) return;

    const layout = event.shiftKey ? SHIFT_KEYBOARD_US : KEYBOARD_US;
    const key = event.key in KEY_TO_CHAR ? KEY_TO_CHAR[event.key] : event.key;

    let scancode = layout.findIndex((e) =>
      typeof e === "string" ? e === key : false,
    );

    if (scancode === -1) {
      scancode = KEY_TO_SCANCODE[event.key] ?? 0;
    }

    callExport("queue_inb_data", 0x60, scancode | 0x80);
    callExport("ps2_keyboard_handler");
  });

  return 0;
}
