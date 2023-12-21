#include "argparse.h"
#include <stdio.h>
#include <stdlib.h>

program_mode parse(int argc, char *argv[]) {
  // interpreter run in interactive mode.
  if (argc < 2) {
    return Interactive;
  } else {
    fprintf(stderr, "[Error] not implemented.\n");
    exit(1);
  }
}
