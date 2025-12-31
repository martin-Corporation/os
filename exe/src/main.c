#include "../include/mrtn.h"
#include <stdio.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s <binary file> [<mrtn binary>]\n", argv[0]);
    return 1;
  }

  if (argc == 2) {
    return inspect_mrtn(argv[1]);
  } else {
    return create_mrtn(argv[1], argv[2]);
  }
}
