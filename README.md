# martinOS

The world's most consistent operating system.

## Running martinOS

### 1. Installing Prerequisites

Install the following packages with [Homebrew](https://brew.sh/):

```sh
brew install i686-elf-grub i686-elf-gcc qemu
```

### 2. Building & Running

After you've installed the required packages, use the `make` command to build and run the OS:

```sh
make run
```

To build for WebAssembly instead:

```sh
make wasm
```

To build an ISO image:

```sh
make iso
```

To delete the build, use:

```sh
make clean
```

## Roadmap

> TODO: Make a GitHub roadmap

- [x] `<canvas>` Renderer for the WASM Build
- [ ] `ext2` File System
- [ ] Userspace (e.g. program execution)
  - [x] Custom Executable Format
  - [ ] Userspace Shell (`msh`)
- [ ] Networking
- [ ] Desktop Environment (DE)
