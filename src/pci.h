#pragma once

#include <stdint.h>

typedef struct pci_device {
  int bus;
  int device;
  int func;
  uint16_t vendor;
  uint16_t device_id;
  uint8_t class_code;
  uint8_t subclass;
} pci_device_t;

typedef struct {
  pci_device_t *devices;
  int count;
} pci_device_list_t;

uint32_t pci_config_read32(uint8_t bus, uint8_t device, uint8_t func,
                           uint8_t offset);
uint16_t pci_get_vendor_id(uint8_t bus, uint8_t device, uint8_t func);
pci_device_list_t pci_get_devices();
