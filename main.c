#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
  int position;
  enum TokenType type;
  char *literal;
};

struct Lexer {
  char *text;
  char *pos;
  struct Token *tokens;
  size_t token_count;
};

void nextToken(struct Lexer *lexer, struct Token *token) {

  char *start = lexer->pos;
  char *literal = malloc(MAX_VAR_NAME_SIZE);
      token->type = ILLEGAL;
      token->literal = literal;
      token->position = lexer->pos - lexer->text;

  while ((*lexer->pos >= 'a' && *lexer->pos <= 'z') ||
         (*lexer->pos >= 'A' && *lexer->pos <= 'Z') ||
         (*lexer->pos >= '0' && *lexer->pos <= '9')) {

    if (lexer->pos - start > MAX_VAR_NAME_SIZE) {
      token->type = ILLEGAL;
      return;
    }
    token->literal[lexer->pos - start] = *lexer->pos;
    lexer->pos++;
    token->type = VARIABLE;
  }

  lexer->pos--;
}

int lex(struct Lexer *lexer) {
  while (true) {
    struct Token token = lexer->tokens[lexer->token_count];
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
      nextToken(lexer, &token);
      break;
    }

    if (!token.literal) {
      char *literal = malloc(2);
      snprintf(literal, 2, "%c", *lexer->pos);
      token.position = lexer->pos - lexer->text,
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


enum FnResultType {
    RESULT_TOKEN,
    RESULT_FUNCTION,
};

union FnResult {
    struct Fn *function;
    struct Token *token;
};
struct FnResultHolder {
    enum FnResultType type;   
    union FnResult *item;
};


struct Fn {
  int param_count ;
  struct Token *param;
  struct FnResultHolder result;
};


enum TokenType peek_token( struct Lexer *lexer , size_t i) {
   if (i < lexer->token_count) {
      return lexer->tokens[i].type;
   }

  fprintf(stderr, "ERROR: index out of bound \n");
  exit(1);
}

int main(void) {

  char *lambda = "($ d . d )";
  struct Token *tokens = malloc(sizeof(struct Token)*MAX_TOKENS);
  struct Lexer lexer = {
      .text = lambda, .pos = lambda, .tokens = tokens, .token_count = 0};

  if(lex(&lexer)) {
    fprintf(stderr, "ERROR: Issue in compilation");
    return 1;
  }
  printf("INFO: Completed lexing. Starting parsing\n");
  struct Fn function = {0};

  enum TokenType nextTokenType ;
  size_t i = 0;
  while  (i < lexer.token_count) {
    if (lexer.tokens[i].type == OPEN_PARAN) {
      nextTokenType = peek_token(&lexer, i +1);
      if ( nextTokenType == LAMBDA) {
        i++;
        // Handle Function Definition

        // Variable - param
        nextTokenType = peek_token(&lexer, i +1);
        if ( nextTokenType != VARIABLE) {
          printf("not variable\n");
          printf("%s\n", lexer.text); for (int j = 0 ; j < lexer.tokens[i+1].position ; ++j) printf("-"); printf("^\n");
          fprintf(stderr, "ERROR: expected VARIABLE, but found %s\n", tokenTypeLiterals[nextTokenType]);
          return 1;
        }
        i++;
        function.param = &lexer.tokens[i];

        // DOT
        nextTokenType = peek_token(&lexer, i +1);
        if ( nextTokenType != DOT) {
          printf("%s\n", lexer.text); for (int j = 0 ; j < lexer.tokens[i+1].position ; ++j) printf("-"); printf("^\n");
          fprintf(stderr, "ERROR: expected DOT, but found %s\n", tokenTypeLiterals[nextTokenType]);
          return 1;
        }
        i++;

        // Variable - result
        nextTokenType = peek_token(&lexer, i +1);
        if ( nextTokenType != VARIABLE) {
          printf("not variable\n");
          printf("%s\n", lexer.text); for (int j = 0 ; j < lexer.tokens[i+1].position ; ++j) printf("-"); printf("^\n");
          fprintf(stderr, "ERROR: expected VARIABLE, but found %s\n", tokenTypeLiterals[nextTokenType]);
          return 1;
        }
        i++;

        union FnResult *result = malloc(100);
        result->token = &lexer.tokens[i];
        function.result.type = RESULT_TOKEN;
        function.result.item =result;

        // ClosedParan
        nextTokenType = peek_token(&lexer, i +1);
        if ( nextTokenType != CLOSE_PARAN) {
          printf("%s\n", lexer.text); for (int j = 0 ; j < lexer.tokens[i+1].position ; ++j) printf("-"); printf("^\n");
          fprintf(stderr, "ERROR: expected CLOSE_PARAN, but found %s\n", tokenTypeLiterals[nextTokenType]);
          return 1;
        }
        i++;
      } else {
        // Handle Function execution 
        printf("Parsing at %d\n", lexer.tokens[i].position);
        //assert(0 && "Function execution not Implemented\n");
        return 1;
      }
    }
    else if (lexer.tokens[i].type == _EOF) {
    } else {
          printf("%s\n", lexer.text); for (int j = 0 ; j <= lexer.tokens[i].position ; ++j) printf("-"); printf("^\n");
      fprintf(stderr, "ERROR: expected function call, but found %s\n", tokenTypeLiterals[lexer.tokens[i].type]);
       return 1;
    }
    i++;
  }

  printf("INFO: Completed parsing. Interpreting the program now\n");
  if (function.result.type == RESULT_TOKEN) {
    struct Token *result = function.result.item->token;
    printf("(λ %s . %s)\n", function.param->literal, result->literal);
    struct Token arg = {
        .type = VARIABLE,
        .literal = "test"
    };
    struct Token result1 = {0};
    if(!strcmp(result->literal, function.param->literal)) {
      result1.type = arg.type;
      result1.literal = arg.literal;
    }
 
    printf("((λ %s . %s) %s)\n", function.param->literal, result->literal, result1.literal);
    printf("Result: %s - %s\n", result1.literal, tokenTypeLiterals[result1.type]);
  } else {
    assert(0 && "Not Implemented");
  }

}
