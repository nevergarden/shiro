#ifndef SHIRO_LEX_H
#define SHIRO_LEX_H

#include <stdint.h>
#include <stdio.h>

typedef enum {
  TokenLBrace,     // {
  TokenRBrace,     // }
  TokenLPran,      // (
  TokenRPran,      // )
  TokenLBrak,      // [
  TokenRBrak,      // ]
  TokenComma,      // ,
  TokenColon,      // :
  TokenSmiColon,   // ;
  TokenDot,        // .
  TokenEq,         // =
  TokenEqEq,       // ==
  TokenNot,        // !
  TokenNEq,        // !=
  TokenLt,         // <
  TokenLEq,        // <=
  TokenShl,        // <<
  TokenShlEq,      // <<=
  TokenGt,         // >
  TokenGEq,        // >=
  TokenShr,        // >>
  TokenShrEq,      // >>=
  TokenPlus,       // +
  TokenPlusEq,     // +=
  TokenPlusPlus,   // ++
  TokenMinus,      // -
  TokenMinusEq,    // -=
  TokenMinusMinus, // --
  TokenMult,       // *
  TokenMultEq,     // *=
  TokenStarDiv,    // */
  TokenDiv,        // /
  TokenDivEq,      // /=
  TokenDivDiv,     // //
  TokenDivStar,    // /*
  TokenMod,        // %
  TokenModEq,      // %=
  TokenOr,         // |
  TokenLOr,        // ||
  TokenOrEq,       // |=
  TokenAnd,        // &
  TokenLAnd,       // &&
  TokenAndEq,      // &=
  TokenXor,        // ^
  TokenXorEq,      // ^=
  TokenNeg,        // ~
  TokenAproxEq,    // ~=
  TokenNumber,     // [0-9]+ | [0-9]+.[0-9]* | 0b[0|1]+ | 0x[0-9a-fA-F]+
  TokenString,     // ".*" | '.*'
  TokenID,         // [$_a-zA-Z][$_a-zA-Z0-9]*
  TokenTrue,       // true
  TokenFalse,      // false
  TokenNull,       // null
  TokenBranch,     // branch
  TokenLoop,       // loop
  TokenFunc,       // function
  TokenReturn,     // return
  TokenBreak,      // break
  TokenContinue,   // continue
  TokenLineFeed,
  TokenEof,

} shiro_token_kind_t;

typedef struct shiro_token_s {
  shiro_token_kind_t kind;
  uint32_t start_pos;
  uint32_t end_pos;
  struct shiro_token_s *prev;
  struct shiro_token_s *next;
} shiro_token_t;

shiro_token_t *init_token_list();
void lexex(long, long, shiro_token_t *, FILE *);
void free_token_list(shiro_token_t *);

void debug_print_token_list(shiro_token_t *, FILE *);

#endif
