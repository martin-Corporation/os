import { defineConfig } from "vite";

export default defineConfig({
  server: {
    allowedHosts: ["martinos-wasm.loca.lt"],
    fs: {
      allow: [".."],
    },
  },
});
