#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKENS 1000
#define MAX_VAR_NAME_SIZE 100

enum TokenType {
  OPEN_PARAN,
  CLOSE_PARAN,
  LAMBDA,
  DOT,
  VARIABLE,
  _EOF,
  WHITE_SPACE,
  ILLEGAL
};

const char *tokenTypeLiterals[] = {"OPEN_PARAN",  "CLOSE_PARAN", "LAMBDA",
                                   "DOT",         "VARIABLE",    "_EOF",
                                   "WHITE_SPACE", "ILLEGAL"};

struct Token {
  enum TokenType type;
  char *literal;
};

struct Lexer {
  char *text;
  char *pos;
  struct Token *tokens;
  size_t token_count;
};

struct Token nextToken(struct Lexer *lexer) {

  char *start = lexer->pos;
  char *literal = malloc(MAX_VAR_NAME_SIZE);
  struct Token token = {
      .type = ILLEGAL,
      .literal = literal,
  };

  while ((*lexer->pos >= 'a' && *lexer->pos <= 'z') ||
         (*lexer->pos >= 'A' && *lexer->pos <= 'Z') ||
         (*lexer->pos >= '0' && *lexer->pos <= '9')) {

    if (lexer->pos - start > MAX_VAR_NAME_SIZE) {
      token.type = ILLEGAL;
      return token;
    }
    token.literal[lexer->pos - start] = *lexer->pos;
    lexer->pos++;
    token.type = VARIABLE;
  }

  lexer->pos--;
  return token;
}

int lex(struct Lexer *lexer) {
  while (true) {
    struct Token token = {0};
    switch (*lexer->pos) {
    case '(':
      token.type = OPEN_PARAN;
      break;
    case ')':
      token.type = CLOSE_PARAN;
      break;
    case '$':
      token.type = LAMBDA;
      break;
    case '.':
      token.type = DOT;
      break;
    case ' ':
    case '\r':
    case '\n':
    case '\b':
      token.type = WHITE_SPACE;
      break;
    case '\0':
      token.type = _EOF;
      break;
    default:
      token = nextToken(lexer);
      break;
    }

    if (!token.literal) {
      char *literal = malloc(2);
      snprintf(literal, 2, "%c", *lexer->pos);
      token.literal = literal;
      //      printf("'%s' : %s: %p\n", token.literal,
      //      tokenTypeLiterals[token.type], (void *)&literal);
    } else {
    }

    if (token.type == WHITE_SPACE) {
      lexer->pos++;
      continue;
    } else if (lexer->token_count < MAX_TOKENS) {
      lexer->tokens[lexer->token_count] = token;
      lexer->token_count++;
    } else {
      fprintf(stderr, "ERROR: Max Token count reached");
      return 1;
    }

    if (token.type == _EOF) {
      break;
    }
    lexer->pos++;
  }
  return 0;
}

int main(void) {

  char *lambda = "($ x . d   )";
  struct Token tokens[MAX_TOKENS] = {0};
  struct Lexer lexer = {
      .text = lambda, .pos = lambda, .tokens = tokens, .token_count = 0};

  lex(&lexer);

  struct Fn {
    struct Token param;
    struct Token result;
  };

  struct Fn functions[10] = {0};
  size_t functions_count = 0;
  for (size_t i = 0; i < lexer.token_count; ++i) {
    if (lexer.tokens[i].type == OPEN_PARAN) {
      if (i + 1 < lexer.token_count && lexer.tokens[i + 1].type == LAMBDA) {
        // Variable - param
        i++;
        if (i + 1 >= lexer.token_count || lexer.tokens[i + 1].type == _EOF) {
          fprintf(stderr, "ERROR: unexpected EOF 1\n");
          return 1;
        }
        if (lexer.tokens[i + 1].type != VARIABLE) {
          fprintf(stderr, "ERROR: expected variable , found %s\n",
                  tokenTypeLiterals[lexer.tokens[i + 1].type]);
          return 1;
        }

        functions[functions_count].param = lexer.tokens[i + 1];

        // DOT
        i++;
        if (i + 1 >= lexer.token_count || lexer.tokens[i + 1].type == _EOF) {
          fprintf(stderr, "ERROR: unexpected EOF2\n");
          return 1;
        }
        if (lexer.tokens[i + 1].type != DOT) {
          fprintf(stderr, "ERROR: expected variable , found %s\n",
                  tokenTypeLiterals[lexer.tokens[i + 1].type]);
          return 1;
        }

        // Variable - result
        i++;
        if (i + 1 >= lexer.token_count || lexer.tokens[i + 1].type == _EOF) {
          fprintf(stderr, "ERROR: unexpected EOF3\n");
          return 1;
        }
        if (lexer.tokens[i + 1].type != VARIABLE) {
          fprintf(stderr, "ERROR: expected variable , found %s\n",
                  tokenTypeLiterals[lexer.tokens[i + 1].type]);
          return 1;
        }

        functions[functions_count].result = lexer.tokens[i + 1];
        // ClosedParan
        i++;
        if (i + 1 >= lexer.token_count || lexer.tokens[i + 1].type == _EOF) {
          fprintf(stderr, "ERROR: unexpected EOF3\n");
          return 1;
        }
        if (lexer.tokens[i + 1].type != CLOSE_PARAN) {
          fprintf(stderr, "ERROR: expected close Param , found %s\n",
                  tokenTypeLiterals[lexer.tokens[i + 1].type]);
          return 1;
        }
        i++;
      }
    }

    else if (lexer.tokens[i].type == _EOF) {

    } else {
      fprintf(stderr, "ERROR: expected function call , found %s\n",
              tokenTypeLiterals[lexer.tokens[i].type]);
      //  return 1;
    }
  }

  //  struct Token arg = {
  //    .type = VARIABLE,
  //    .literal = "test"
  //  };

  struct Fn fn1 = functions[0];
  printf("(λ %s . %s)", fn1.param.literal, fn1.result.literal);
}
