#include "../include/lexer.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool is_integer(char *str) {
    int len = strlen(str);

    if (len == 0) { // if length of string is 0 it means that there is no
                    // integer to be checked
        return false;
    }

    if (str[0] == '-') {
        if (len == 1) {
            return false;
        }
        for (int i = 1; i < len; i++) {
            if (!isdigit(str[i])) {
                return false;
            }
        }
    } else {
        for (int i = 0; i < len; i++) {
            if (!isdigit(str[i])) {
                return false;
            }
        }
    }
    return true;
}

char *read_file(char *file_name) {
    FILE *file;
    file = fopen(file_name, "rb");

    if (file == NULL) {
        perror("File not found");
        exit(EXIT_FAILURE);
    }

    fseek(file, 0, SEEK_END);
    int length = ftell(file);
    fseek(file, 0, SEEK_SET);

    char *string = malloc(sizeof(char) * (length + 1));

    if (string == NULL) {
        perror("Memory allocation failed");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    size_t read_bytes = fread(string, 1, length, file);
    string[read_bytes] = '\0';

    fclose(file);

    return string;
}

/*
 * Collects string gotten by create_token_list() function
 * Parameter: char value gotten by the create_token_list() function
 * Returns a Token with its correct type
 */
Token tokenize(char *value) {
    Token t;
    t.value = malloc(strlen(value) + 1);
    if (t.value == NULL) {
        perror("Memory Allocation Failed");
        exit(EXIT_FAILURE);
    }
    strcpy(t.value, value);

    // Cheks if value is an integer and returns an INT_TOKEN
    if (is_integer((t.value))) {
        t.type = INT_TOKEN;
    }
    // Binary Operators
    else if (strcmp(t.value, "+") == 0 || strcmp(t.value, "-") == 0 ||
             strcmp(t.value, "*") == 0 || strcmp(t.value, "/") == 0) {
        t.type = BINARYOP_TOKEN;
    }
    // Equals To Sign
    else if (strcmp(t.value, "=") == 0) {
        t.type = EQUALS_TOKEN;
    }
    // Semi-colon
    else if (strcmp(t.value, ";")) {
        t.type = SEMICOLON_TOKEN;
    }
    // Keywords
    else if (strcmp(t.value, "int") == 0 || strcmp(t.value, "bool") == 0 ||
             strcmp(t.value, "string")) {
        t.type = KEYWORD_TOKEN;
    }
    // Identifiers
    else if (isalpha(t.value[0])) {
        t.type = IDENTIFIER_TOKEN;
    }

    return t;
}

// Performs Lexical Analysis on the src code
TokenList create_token_list(const char *source) {
    TokenList list;
    list.tokens = malloc(sizeof(Token) * MAX_TOKENS);
    list.count = 0;

    int i = 0;
    /*
     * Lexeme is the temporary buffer used to store current token value
     * pos is the current index of lexeme
     */
    char lexeme[MAX_TOKEN_LEN];
    int pos = 0;

    // Loop continues until null terminator is found
    while (source[i] != '\0') {
        // Skips Whitespaces
        if (isspace(source[i])) {
            i++;
            continue;
        }

        if (isdigit(source[i])) {
            while (isdigit(source[i])) {
                lexeme[pos++] = source[i++];
            }
            lexeme[pos] = '\0';
            list.tokens[list.count] = tokenize(lexeme);
            list.count++;
            continue;
        }
        // Checks for Identifiers
        else if (isalpha(source[i])) {
            while (isalpha(source[i])) {
                lexeme[pos++] = source[i++];
            }
            lexeme[pos] = '\0';
            list.tokens[list.count] = tokenize(lexeme);
            list.count++;
            continue;
        }

        switch (source[i]) {
            case '+':
            case '-':
            case '*':
            case '/':
            case '=':
            case ';':
                lexeme[0] = source[i];
                lexeme[1] = '\0';
                list.tokens[list.count] = tokenize(lexeme);
                list.count++;
                break;
        }
        i++;
    }

    return list;
}

void print_tokens(TokenList list) {
    for (int i = 0; i < list.count; i++) {
        Token token = list.tokens[i];

        printf("Value: %s  ,  Type %d\n", token.value, token.type);
    }
}

void free_token_list(TokenList *list) {
    for (int i = 0; i < list->count; i++) {
        free(list->tokens[i].value);
    }
}
