import "./style.css";
import { loadOS } from "./loader";
import { startRenderer } from "./terminal/buffer";

async function main() {
  try {
    const instance = await loadOS();
    startRenderer(instance);

    if (instance.exports.kmain) {
      (instance.exports.kmain as Function)();
    }
  } catch (err) {
    console.error("Failed to load OS:", err);
    document.querySelector("canvas")!.remove();
    document.body.innerHTML += `<p>Couldn't load OS. See console for details.<br />
<pre style="font: inherit">${err instanceof Error ? err : JSON.stringify(err)}</pre></p>`;
  }
}

main();
