#include "pci.h"
#include <asm/io.h>
#include <stdint.h>

#ifdef __wasm__
#warning "PCI API unsupported on WASM"
#endif

static void pci_write_address(uint32_t addr) {
#ifndef __wasm__
  outl(0xCF8, 0x80000000 | addr);
#endif
}

static uint32_t pci_read_data() {
#ifndef __wasm__
  return inl(0xCFC);
#else
  return 0;
#endif
}

uint32_t pci_config_read32(uint8_t bus, uint8_t device, uint8_t func,
                           uint8_t offset) {
  uint32_t addr = (bus << 16) | (device << 11) | (func << 8) | (offset & ~3);
  pci_write_address(addr);
  return pci_read_data();
}

uint16_t pci_get_vendor_id(uint8_t bus, uint8_t device, uint8_t func) {
  uint32_t reg = pci_config_read32(bus, device, func, 0x00);
  return reg & 0xFFFF;
}

pci_device_list_t pci_get_devices() {
  static pci_device_t devices[256 * 32 * 8];
  static int count = 0;

#ifndef __wasm__
  for (int bus = 0; bus < 256; bus++) {
    for (int device = 0; device < 32; device++) {
      for (int func = 0; func < 8; func++) {
        uint16_t vendor = pci_get_vendor_id(bus, device, func);

        if (vendor == 0xFFFF) {
          // TODO: Multi-function devices
          if (func == 0)
            break;

          continue;
        }

        uint32_t id_reg = pci_config_read32(bus, device, func, 0x00);
        uint16_t device_id = id_reg >> 16;
        uint32_t class_reg = pci_config_read32(bus, device, func, 0x08);
        uint8_t class_code = class_reg >> 24;
        uint8_t subclass = (class_reg >> 16) & 0xFF;

        devices[count].bus = bus;
        devices[count].device = device;
        devices[count].func = func;
        devices[count].vendor = vendor;
        devices[count].device_id = device_id;
        devices[count].class_code = class_code;
        devices[count].subclass = subclass;

        count++;
      }
    }
  }
#endif

  return (pci_device_list_t){devices, count};
}
