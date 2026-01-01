#include "mrtn.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

int create_mrtn(const char *bin_path, const char *mrtn_path) {
  FILE *bin = fopen(bin_path, "rb");

  if (!bin) {
    perror("fopen bin");
    return -1;
  }

  FILE *mrtn = fopen(mrtn_path, "wb");

  if (!mrtn) {
    perror("fopen mrtn");
    fclose(bin);
    return -2;
  }

  fseek(bin, 0, SEEK_END);
  uint32_t size = (uint32_t)ftell(bin);
  fseek(bin, 0, SEEK_SET);

  fwrite(MAGIC, 1, MAGIC_SIZE, mrtn);
  fwrite(&(unsigned char){VERSION}, 1, 1, mrtn);
  fwrite(&size, 4, 1, mrtn);

  unsigned char buffer[4096];
  size_t read;

  while ((read = fread(buffer, 1, sizeof(buffer), bin)) > 0) {
    fwrite(buffer, 1, read, mrtn);
  }

  fclose(bin);
  fclose(mrtn);

  return 0;
}

int inspect_mrtn(const char *mrtn_path) {
  FILE *f = fopen(mrtn_path, "rb");

  if (!f) {
    perror("fopen");
    return -1;
  }

  char magic[MAGIC_SIZE];

  if (fread(magic, 1, MAGIC_SIZE, f) != MAGIC_SIZE) {
    perror("fread magic");
    fclose(f);
    return -2;
  }

  if (magic[0] != 'M' || magic[1] != 'R' || magic[2] != 'T' ||
      magic[3] != 'N') {
    fprintf(stderr, "Invalid magic\n");
    fclose(f);
    return -3;
  }

  uint8_t version;

  if (fread(&version, 1, 1, f) != 1) {
    perror("fread version");
    fclose(f);
    return -4;
  }

  printf("Version: %u\n", version);

  uint32_t size;

  if (fread(&size, 4, 1, f) != 1) {
    perror("fread size");
    fclose(f);
    return -5;
  }

  printf("Payload size: %u bytes\n", size);

  unsigned char *payload = malloc(size);

  if (!payload) {
    perror("malloc");
    fclose(f);
    return -6;
  }

  if (fread(payload, 1, size, f) != size) {
    perror("fread payload");
    free(payload);
    fclose(f);
    return -7;
  }

  printf("Payload first 16 bytes: ");

  for (uint32_t i = 0; i < 16 && i < size; i++)
    printf("%02x ", payload[i]);

  printf("\n");

  free(payload);
  fclose(f);
  return 0;
}
