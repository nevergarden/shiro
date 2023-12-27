#include "shell.h"
#include "lex.h"
#include "version.h"

#include <stdio.h>
#include <stdlib.h>

#include <readline/readline.h>
#include <readline/history.h>

static uint32_t current_line = 0;
static char *line_read = (char *)NULL;
static shiro_token_t *tokens = (shiro_token_t *)NULL;

void handle_input(FILE *source) {
  long start_pos = ftell(source);
  fwrite(line_read, strlen(line_read), sizeof(char), source);
  fputc('\n', source);
  long end_pos = ftell(source);
  lexex(start_pos, end_pos, tokens, source);
  debug_print_token_list(tokens, source);
  fseek(source, 0, SEEK_END);
  add_history(line_read);
}

void interactive_shell() {
  printf("Shiro %s interactive shell\n", version_string);
  FILE *interactive_source = tmpfile();
  tokens = init_token_list();

  while (1) {
    line_read = readline(">> ");
    if (!line_read)
      break;

    handle_input(interactive_source);
  }
}
