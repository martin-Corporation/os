# martinOS Executable Format (`.mrtn`)

This directory contains the implementation of the martinOS executable format (`.mrtn`).

## Specification

| Offset | Size   | Data                                    |
| ------ | ------ | --------------------------------------- |
| 0x00   | 4      | File Signature (ASCII "MRTN")           |
| 0x04   | 1      | Version                                 |
| 0x05   | 4      | Payload Size (little-endian `uint32_t`) |
| 0x09   | @ 0x05 | ELF Payload                             |
