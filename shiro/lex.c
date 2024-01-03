#include "lex.h"
#include <ctype.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static char string_initializer = '\0';
static uint32_t current = 0;
static uint32_t length = 0;
static uint32_t literal_start_pos = 0;

shiro_token_t *init_token_list() {
  shiro_token_t *head = malloc(sizeof(shiro_token_t));
  head->kind = -1;
  head->start_pos = 0;
  head->end_pos = 0;
  head->prev = NULL;
  head->next = NULL;
  return head;
}

void free_token_list(shiro_token_t *head) {
  shiro_token_t *next = NULL;
  while (head->next != NULL) {
    next = head->next;
    free(head);
    head = next;
  }
  free(head);
}

uint8_t is_id_starter(char c) { return isalpha(c) || c == '_' || c == '$'; }
uint8_t is_id_char(char c) { return is_id_starter(c) || isdigit(c); }

void add_token(shiro_token_t *list, shiro_token_kind_t kind, uint32_t start_pos,
               uint32_t end_pos) {
  shiro_token_t *current = list;
  while (current->next != NULL) {
    current = current->next;
  }
  current->next = malloc(sizeof(shiro_token_t));
  current->next->kind = kind;
  current->next->start_pos = start_pos;
  current->next->end_pos = end_pos;
  current->next->prev = current;
  current->next->next = NULL;
}

void string(size_t line_len, char *line, shiro_token_t *list) {
  while (current < line_len && line[current] != string_initializer) {
    current += 1;
    length += 1;
  }

  if (line[current] == string_initializer) {
    string_initializer = '\0';
    current += 1;
    add_token(list, TokenString, literal_start_pos, literal_start_pos + length);
  }
}

void number(size_t line_len, char *line, shiro_token_t *list) {
  uint8_t isHex = 0;
  uint8_t isBin = 0;

  if (line[current] == '0' && current + 1 < line_len) {
    if (line[current + 1] == 'x') {
      isHex = 1;
      current += 2;
      length += 2;
    } else if (line[current + 1] == 'b') {
      isBin = 1;
      current += 2;
      length += 2;
    }
    while (current < line_len && isdigit(line[current])) {
      current += 1;
      length += 1;
    }
  }
  add_token(list, TokenNumber, literal_start_pos, literal_start_pos + length);
}

int8_t id_or_keyword(size_t line_len, char *line, shiro_token_t *list) {
  uint32_t start_pos = current;
  if (current < line_len && is_id_starter(line[current])) {
    current += 1;
    length += 1;
  } else {
    return -1;
  }

  while (current < line_len && is_id_char(line[current])) {
    current += 1;
    length += 1;
  }

  char *string = malloc(length * sizeof(char));
  strncpy(string, line + current, length);

  if (strcmp("true", string) == 0) {
    add_token(list, TokenTrue, literal_start_pos, literal_start_pos + length);
  } else if (strcmp("false", string) == 0) {
    add_token(list, TokenFalse, literal_start_pos, literal_start_pos + length);
  } else if (strcmp("null", string) == 0) {
    add_token(list, TokenNull, literal_start_pos, literal_start_pos + length);
  } else if (strcmp("branch", string) == 0) {
    add_token(list, TokenBranch, literal_start_pos, literal_start_pos + length);
  } else if (strcmp("loop", string) == 0) {
    add_token(list, TokenLoop, literal_start_pos, literal_start_pos + length);
  } else if (strcmp("function", string) == 0) {
    add_token(list, TokenFunc, literal_start_pos, literal_start_pos + length);
  } else if (strcmp("return", string) == 0) {
    add_token(list, TokenReturn, literal_start_pos, literal_start_pos + length);
  } else if (strcmp("break", string) == 0) {
    add_token(list, TokenBreak, literal_start_pos, literal_start_pos + length);
  } else if (strcmp("continue", string) == 0) {
    add_token(list, TokenContinue, literal_start_pos,
              literal_start_pos + length);
  } else {
    add_token(list, TokenID, literal_start_pos, literal_start_pos + length);
  }
  free(string);
  return 0;
}

void lexex(long start_pos, long end_pos, shiro_token_t *head, FILE *source) {
  size_t line_len = end_pos - start_pos;
  char *line = malloc(line_len);
  fseek(source, start_pos, SEEK_SET);
  fread(line, line_len, sizeof(char), source);

  current = 0;
  if (string_initializer == '\0') {
    length = 0;
  } else {
    string(line_len, line, head);
  }
  while (current < line_len) {
    length = 0;
    switch (line[current]) {
    case '{':
      add_token(head, TokenLBrace, current + start_pos,
                current + start_pos + 1);
      current += 1;
      break;
    case '}':
      add_token(head, TokenRBrace, current + start_pos,
                current + start_pos + 1);
      current += 1;
      break;
    case '(':
      add_token(head, TokenLPran, current + start_pos, current + start_pos + 1);
      current += 1;
      break;
    case ')':
      add_token(head, TokenRPran, current + start_pos, current + start_pos + 1);
      current += 1;
      break;
    case '[':
      add_token(head, TokenLBrak, current + start_pos, current + start_pos + 1);
      current += 1;
      break;
    case ']':
      add_token(head, TokenRBrak, current + start_pos, current + start_pos + 1);
      current += 1;
      break;
    case ',':
      add_token(head, TokenComma, current + start_pos, current + start_pos + 1);
      current += 1;
      break;
    case ':':
      add_token(head, TokenColon, current + start_pos, current + start_pos + 1);
      current += 1;
      break;
    case ';':
      add_token(head, TokenSmiColon, current + start_pos,
                current + start_pos + 1);
      current += 1;
      break;
    case '.':
      add_token(head, TokenDot, current + start_pos, current + start_pos + 1);
      current += 1;
      break;
    case '=':
      if (current + 1 < line_len && line[current + 1] == '=') {
        add_token(head, TokenEqEq, current + start_pos,
                  current + start_pos + 2);
        current += 2;
        break;
      }
      add_token(head, TokenEq, current + start_pos, current + start_pos + 1);
      current += 1;
      break;
    case '!':
      if (current + 1 < line_len) {
        if (line[current + 1] == '=') {
          add_token(head, TokenNEq, current + start_pos,
                    current + start_pos + 2);
          current += 2;
          break;
        }
      }
      add_token(head, TokenNot, current + start_pos, current + start_pos + 1);
      current += 1;
      break;
    case '<':
      if (current + 1 < line_len) {
        if (line[current + 1] == '=') {
          add_token(head, TokenLEq, current + start_pos,
                    current + start_pos + 2);
          current += 2;
          break;
        } else if (line[current + 1] == '<') {
          if (current + 2 < line_len && line[current + 2] == '=') {
            add_token(head, TokenShlEq, current + start_pos,
                      current + start_pos + 3);
            current += 3;
            break;
          }
          add_token(head, TokenShl, current + start_pos,
                    current + start_pos + 2);
          current += 2;
          break;
        }
      }
      add_token(head, TokenLt, current + start_pos, current + start_pos + 1);
      current += 1;
      break;
    case '>':
      if (current + 1 < line_len) {
        if (line[current + 1] == '=') {
          add_token(head, TokenGEq, current + start_pos,
                    current + start_pos + 2);
          current += 2;
          break;
        } else if (line[current + 1] == '>') {
          if (current + 2 < line_len && line[current + 2] == '=') {
            add_token(head, TokenShrEq, current + start_pos,
                      current + start_pos + 3);
            current += 3;
            break;
          }
          add_token(head, TokenShr, current + start_pos,
                    current + start_pos + 2);
          current += 2;
          break;
        }
      }
      add_token(head, TokenGt, current + start_pos, current + start_pos + 1);
      current += 1;
      break;
    case '+':
      if (current + 1 < line_len) {
        if (line[current + 1] == '=') {
          add_token(head, TokenPlusEq, current + start_pos,
                    current + start_pos + 2);
          current += 2;
          break;
        } else if (line[current + 1] == '+') {
          add_token(head, TokenPlusPlus, current + start_pos,
                    current + start_pos + 2);
          current += 2;
          break;
        }
      }
      add_token(head, TokenPlus, current + start_pos, current + start_pos + 1);
      current += 1;
      break;
    case '-':
      if (current + 1 < line_len) {
        if (line[current + 1] == '=') {
          add_token(head, TokenMinusEq, current + start_pos,
                    current + start_pos + 2);
          current += 2;
          break;
        } else if (line[current + 1] == '-') {
          add_token(head, TokenMinusMinus, current + start_pos,
                    current + start_pos + 2);
          current += 2;
          break;
        }
      }
      add_token(head, TokenMinus, current + start_pos, current + start_pos + 1);
      current += 1;
      break;
    case '*':
      if (current + 1 < line_len) {
        if (line[current + 1] == '=') {
          add_token(head, TokenMultEq, current + start_pos,
                    current + start_pos + 2);
          current += 2;
          break;
        } else if (line[current + 1] == '/') {
          add_token(head, TokenStarDiv, current + start_pos,
                    current + start_pos + 2);
          current += 2;
          break;
        }
      }
      add_token(head, TokenMult, current + start_pos, current + start_pos + 1);
      current += 1;
      break;
    case '/':
      if (current + 1 < line_len) {
        if (line[current + 1] == '=') {
          add_token(head, TokenDivEq, current + start_pos,
                    current + start_pos + 2);
          current += 2;
          break;
        } else if (line[current + 1] == '*') {
          add_token(head, TokenDivStar, current + start_pos,
                    current + start_pos + 2);
          current += 2;
          break;
        } else if (line[current + 1] == '/') {
          add_token(head, TokenDivDiv, current + start_pos,
                    current + start_pos + 2);
          current += 2;
          break;
        }
        add_token(head, TokenDiv, current + start_pos, current + start_pos + 1);
        current += 1;
        break;
      case '%':
        if (current + 1 < line_len) {
          if (line[current + 1] == '=') {
            add_token(head, TokenModEq, current + start_pos,
                      current + start_pos + 2);
            current += 2;
            break;
          }
        }
        add_token(head, TokenMod, current + start_pos, current + start_pos + 1);
        current += 1;
        break;
      case '|':
        if (current + 1 < line_len) {
          if (line[current + 1] == '|') {
            add_token(head, TokenLOr, current + start_pos,
                      current + start_pos + 2);
            current += 2;
            break;
          } else if (line[current + 1] == '=') {
            add_token(head, TokenOrEq, current + start_pos,
                      current + start_pos + 2);
            current += 2;
            break;
          }
        }
        add_token(head, TokenOr, current + start_pos, current + start_pos + 1);
        current += 1;
        break;
      case '&':
        if (current + 1 < line_len) {
          if (line[current + 1] == '&') {
            add_token(head, TokenLAnd, current + start_pos,
                      current + start_pos + 2);
            current += 2;
            break;
          } else if (line[current + 1] == '=') {
            add_token(head, TokenAndEq, current + start_pos,
                      current + start_pos + 2);
            current += 2;
            break;
          }
        }
        add_token(head, TokenAnd, current + start_pos, current + start_pos + 1);
        current += 1;
        break;
      case '^':
        if (current + 1 < line_len) {
          if (line[current + 1] == '=') {
            add_token(head, TokenXorEq, current + start_pos,
                      current + start_pos + 2);
            current += 2;
            break;
          }
        }
        add_token(head, TokenXor, current + start_pos, current + start_pos + 1);
        current += 1;
        break;
      case '~':
        if (current + 1 < line_len) {
          if (line[current + 1] == '=') {
            add_token(head, TokenAproxEq, current + start_pos,
                      current + start_pos + 2);
            current += 2;
            break;
          }
        }
        add_token(head, TokenNeg, current + start_pos, current + start_pos + 1);
        current += 1;
        break;
      case '\'':
        string_initializer = '\'';
        current += 1;
        literal_start_pos = start_pos + current;
        string(line_len, line, head);
        break;
      case '"':
        string_initializer = '"';
        current += 1;
        literal_start_pos = start_pos + current;
        string(line_len, line, head);
        break;
      case ' ':
      case '\t':
      case '\r':
        current += 1;
        break;

      case '\n':
        add_token(head, TokenLineFeed, current + start_pos,
                  current + start_pos + 1);
        current += 1;
        break;
      default:
        literal_start_pos = start_pos + current;
        if (isdigit(line[current])) {
          number(line_len, line, head);
          break;
        } else {
          if (id_or_keyword(line_len, line, head) < 0) {
            exit(-1);
          }
          break;
        }
      }
    }
  }
  free(line);
}

void debug_print_token(shiro_token_t *token, int is_last, FILE *source) {
  fseek(source, token->start_pos, SEEK_SET);
  char *literal = malloc(token->end_pos - token->start_pos + 1);
  fread(literal, token->end_pos - token->start_pos, sizeof(char), source);
  literal[token->end_pos + token->start_pos + 1] = '\0';
  printf("(%d: %s, %d, %d)", token->kind, literal, token->start_pos,
         token->end_pos);
  free(literal);

  if (is_last) {
    printf("\n");
  } else {
    printf(" ");
  }
}

void debug_print_token_list(shiro_token_t *list, FILE *source) {
  shiro_token_t *current = list;
  while (current->next != NULL) {
    debug_print_token(current, 0, source);
    current = current->next;
  }
  debug_print_token(current, 1, source);
}
