#ifndef LEXER_H
#define LEXER_H

#include <stdbool.h>

#define MAX_TOKENS 128
#define MAX_TOKEN_LEN 256
typedef enum {
        INT_TOKEN,
        BINARYOP_TOKEN,
        EQUALS_TOKEN,
        KEYWORD_TOKEN,
        SEMICOLON_TOKEN,
        STRING_TOKEN,
        IDENTIFIER_TOKEN,
        L_PARENTHESIS,
        R_PARENTHESIS,
        BOOL_TOKEN,
        EOF_TOKEN
} TokenType;
typedef struct {
        TokenType type;
        char *value;
} Token;

typedef struct {
        Token *tokens;
        int count;
} TokenList;

char *read_file(char *file_name);
TokenList create_token_list(const char *source);
bool is_integer(char *str);
void print_tokens(TokenList list);

void free_token_list(TokenList *list);
#endif // !LEXER_H
