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



enum BlockType {
  BLOCK_FUNCTION,
  BLOCK_APPLICATION
};

struct FnApplication {
  struct Fn *function;
  struct Token *params;
  int params_count;
};

union BlockElem {
  struct Fn *function;
  struct FnApplication *application;

};

struct Block {
  enum BlockType type;
  union BlockElem elem;
};

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

  char *lambda = "( ($ d . d ) test )";
  struct Token *tokens = malloc(sizeof(struct Token)*MAX_TOKENS);
  struct Lexer lexer = {
      .text = lambda, .pos = lambda, .tokens = tokens, .token_count = 0};

  if(lex(&lexer)) {
    fprintf(stderr, "ERROR: Issue in compilation");
    return 1;
  }
  printf("INFO: Completed lexing. Starting parsing\n");
  struct Block block = {0};

  enum TokenType nextTokenType ;
  size_t i = 0;
  while  (i < lexer.token_count) {
    if (lexer.tokens[i].type == OPEN_PARAN) {
      nextTokenType = peek_token(&lexer, i +1);
      if ( nextTokenType == LAMBDA) {
        if(block.type != BLOCK_APPLICATION) {
          block.type = BLOCK_FUNCTION;
        }
        i++;
        // Handle Function Definition

        // Variable - param
        nextTokenType = peek_token(&lexer, i +1);
        if ( nextTokenType != VARIABLE) {
          printf("%s\n", lexer.text); for (int j = 0 ; j < lexer.tokens[i+1].position ; ++j) printf("-"); printf("^\n");
          fprintf(stderr, "ERROR: expected VARIABLE, but found %s\n", tokenTypeLiterals[nextTokenType]);
          return 1;
        }
        i++;

        struct Fn *function = malloc(sizeof (struct Fn));

        function->param = &lexer.tokens[i];
        function->param_count++;

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
          printf("%s\n", lexer.text); for (int j = 0 ; j < lexer.tokens[i+1].position ; ++j) printf("-"); printf("^\n");
          fprintf(stderr, "ERROR: expected VARIABLE, but found %s\n", tokenTypeLiterals[nextTokenType]);
          return 1;
        }
        i++;

        union FnResult *result = malloc(100);
        result->token = &lexer.tokens[i];
        function->result.type = RESULT_TOKEN;
        function->result.item =result;

        if(block.type == BLOCK_APPLICATION && block.elem.application) {
          block.elem.application->function = function;
        } else {
          block.elem.function = function;
        }


        // ClosedParan
        nextTokenType = peek_token(&lexer, i +1);
        if ( nextTokenType != CLOSE_PARAN) {
          printf("%s\n", lexer.text); for (int j = 0 ; j < lexer.tokens[i+1].position ; ++j) printf("-"); printf("^\n");
          fprintf(stderr, "ERROR: expected CLOSE_PARAN, but found %s\n", tokenTypeLiterals[nextTokenType]);
          return 1;
        }
        i++;
      } else {
        block.type = BLOCK_APPLICATION;
        // Handle Function execution 
        //printf("%s\n", lexer.text); for (int j = 0 ; j < lexer.tokens[i].position ; ++j) printf("-"); printf("^\n");
        struct FnApplication *application = malloc(sizeof(struct FnApplication));
        block.elem.application = application;
      }
    }
    else if (lexer.tokens[i].type == _EOF) {
      if(block.type == BLOCK_APPLICATION && block.elem.application && !block.elem.application->params) {
        fprintf(stderr, "ERROR: expected function call, but found %s\n", tokenTypeLiterals[lexer.tokens[i].type]);
      }
    } else if (lexer.tokens[i].type == VARIABLE) {
       if(block.type == BLOCK_APPLICATION && block.elem.application ) {
          block.elem.application->params = &lexer.tokens[i];
          block.elem.application->params_count++;
       }
    }
    else if (lexer.tokens[i].type == CLOSE_PARAN) {
    }
    else {
          printf("%s\n", lexer.text); for (int j = 0 ; j <= lexer.tokens[i].position ; ++j) printf("-"); printf("^\n");
      fprintf(stderr, "ERROR: expected function call, but found %s\n", tokenTypeLiterals[lexer.tokens[i].type]);
       return 1;
    }
    i++;
  }


  printf("INFO: Completed parsing. Interpreting the program now\n");
  if (block.type == BLOCK_FUNCTION) {
    struct Fn function = *block.elem.function;
    struct Token *result = function.result.item->token;
    printf("(λ %s . %s)\n", function.param->literal, result->literal);
  } else {
    struct Token arg = *block.elem.application->params; 
    struct Fn function = *block.elem.application->function;
    struct FnResultHolder result = function.result;
    if (result.type == RESULT_TOKEN)  {
      if (strcmp(result.item->token->literal, function.param->literal) == 0) {
        printf("Result: %s - %s\n", arg.literal, tokenTypeLiterals[arg.type]);
      } else {
        struct Fn result_function = *function.result.item->function; 
        printf("(λ %s . %s)\n", result_function.param->literal, result_function.result.item->token->literal);
        assert(0 && "UNKOWN TOKEN NOT IMPLEMENTED");
      }
    } else {
        assert(0 && "RESULT_FUNCTION NOT IMPLEMENTED");
    }
    
  }
  //if (function.result.type == RESULT_TOKEN) {
  //  struct Token *result = function.result.item->token;
  //  printf("(λ %s . %s)\n", function.param->literal, result->literal);
  //  struct Token arg = {
  //      .type = VARIABLE,
  //      .literal = "test"
  //  };
  //  struct Token result1 = {0};
  //  if(!strcmp(result->literal, function.param->literal)) {
  //    result1.type = arg.type;
  //    result1.literal = arg.literal;
  //  }
 
  //  printf("((λ %s . %s) %s)\n", function.param->literal, result->literal, result1.literal);
  //  printf("Result: %s - %s\n", result1.literal, tokenTypeLiterals[result1.type]);
  //} else {
  //  assert(0 && "Not Implemented");
  //}

}
