#include "shell.h"
#include "version.h"

#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

static char *line_read = (char *)NULL;

void interactive_shell() {
  printf("Shiro %s interactive shell\n", version_string);

  while (1) {
    line_read = readline(">> ");
    if (!line_read)
      break;
    add_history(line_read);

    printf("%s\n", line_read);
  }
}
