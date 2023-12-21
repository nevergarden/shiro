#include "argparse.h"
#include "shell.h"

int main(int argc, char *argv[]) {
  // Argument Parsing
  switch (parse(argc, argv)) {
  case Interactive:
    interactive_shell();
  }
  return 0;
}
