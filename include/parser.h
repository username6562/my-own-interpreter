#ifndef PARSER_H
#define PARSER_H

#include "../include/lexer.h"

typedef enum {
        INT_NODE,
        ADDITION_NODE,
        SUBTRACTION_NODE,
        MULTIPLICATION_NODE,
        DIVISION_NODE,
        ASSIGNMENT_NODE,
        IDENTIFIER_NODE,
} NodeType;

typedef struct ASTNode ASTNode;

struct ASTNode {
        char *value;
        ASTNode *left;
        ASTNode *right;
        NodeType type;
};

ASTNode *parse_statement(TokenList list, int *pos);
ASTNode *create_node(char *value, NodeType type);
Token get_next_token(TokenList list, int *pos);

void print_ast_tree(ASTNode *root);
#endif // !PARSER_H
