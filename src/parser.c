#include "../include/parser.h"
#include "../include/lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

ASTNode *create_node(char *value, NodeType type) {
        ASTNode *node = malloc(sizeof(ASTNode));

        if (node == NULL) {
                perror("Memory Allocation Failed");
                exit(EXIT_FAILURE);
        }
        node->value = value;
        node->left = NULL;
        node->right = NULL;
        node->type = type;
        node->child_count = 0;

        return node;
}

/*
 * Parameters
 *  - list: current array of tokens
 *  - pos: current index of array
 */
Token get_current_token(TokenList list, int *pos) {
        Token token = list.tokens[*pos];
        return token;
}

Token get_next_token(TokenList list, int *pos) {
        Token t = list.tokens[++(*pos)];
        return t;
}

Token get_prev_token(TokenList list, int *pos) {
        Token t = list.tokens[--(*pos)];
        return t;
}

/*
 *  Handles base balues like literals Identifiers , numbers
 *  Returns node node matching the token type
 */
ASTNode *parse_primary(TokenList list, int *pos) {
        Token next_tok = get_next_token(list, pos);

        if (next_tok.type == INT_TOKEN) {
                ASTNode *node = create_node(next_tok.value, INT_NODE);
                return node;
        }
        else if (next_tok.type == IDENTIFIER_TOKEN) {
                ASTNode *node = create_node(next_tok.value, IDENTIFIER_NODE);
                return node;
        }
        else if (next_tok.type == STRING_TOKEN) {
                ASTNode *node = create_node(next_tok.value, STRING_NODE);
                return node;
        }

        return NULL;
}

int get_binding_power(Token token) {
        if (token.type == BINARYOP_TOKEN) {
                if (strcmp(token.value, "+") == 0 ||
                    strcmp(token.value, "-") == 0) {
                        return 10;
                }
                else {
                        return 20;
                }
        }
        else if (token.type == EQUALS_TOKEN) {
                return 5;
        }
        return 0;
}

/*
 * Parameters: collect a list of tokens in their order and returns a fully
 * functional Node Tree which will get evaluated by the evaluator For more
 * context on how it works go to  ../docs/Parser.md
 */
ASTNode *parse_expr(TokenList list, int *pos, int current_binding_power) {
        ASTNode *left = parse_primary(list, pos);

        while (true) {
                int saved_pos = *pos;
                Token next_tok = get_next_token(list, pos);
                int next_bp = get_binding_power(next_tok);

                if (current_binding_power >= next_bp) {
                        *pos = saved_pos;
                        break;
                }
                ASTNode *opNode;
                if (next_tok.type == BINARYOP_TOKEN) {
                        if (strcmp(next_tok.value, "+") == 0) {

                                opNode =
                                    create_node(next_tok.value, ADDITION_NODE);
                        }
                        else if (strcmp(next_tok.value, "-") == 0) {
                                opNode = create_node(next_tok.value,
                                                     SUBTRACTION_NODE);
                        }
                        else if (strcmp(next_tok.value, "*") == 0) {
                                opNode = create_node(next_tok.value,
                                                     MULTIPLICATION_NODE);
                        }
                        else if (strcmp(next_tok.value, "/") == 0) {
                                opNode =
                                    create_node(next_tok.value, DIVISION_NODE);
                        }
                }
                else if (next_tok.type == EQUALS_TOKEN) {
                        opNode = create_node(next_tok.value, ASSIGNMENT_NODE);
                }
                else {
                        break;
                }
                opNode->left = left;
                opNode->right = parse_expr(list, pos, next_bp);
                left = opNode;
        }

        return left;
}

// Inputs a stream of token and makes a Tree Node of Variable Assignments and
// Declaration
ASTNode *parse_var_decl(TokenList list, int *pos) {
        Token next_token = get_next_token(list, pos);

        //  Makes an ASTNode for when a variable is  declared
        if (next_token.type == IDENTIFIER_TOKEN) {
                ASTNode *variable_name =
                    create_node(next_token.value, IDENTIFIER_NODE);
                Token equals_token = get_next_token(list, pos);

                if (equals_token.type == EQUALS_TOKEN) {
                        ASTNode *eq_node =
                            create_node(equals_token.value, ASSIGNMENT_NODE);
                        ASTNode *expr_node = parse_expr(list, pos, 0);
                        eq_node->left = variable_name;
                        eq_node->right = expr_node;

                        Token semi_colontok = get_next_token(
                            list, pos); // get_current_token is used here
                                        // instead of get_next_token because
                                        // parse_expr already looks forward
                        if (semi_colontok.type == SEMICOLON_TOKEN) {
                                get_current_token(list, pos);
                        }
                        else {
                                perror("Syntax Error: Semi-Colon Expected\n");
                                exit(EXIT_FAILURE);
                        }

                        return eq_node;
                }
        }
        // Makes an AST if a variable has been assigned
        else if (next_token.type == EQUALS_TOKEN) {
                Token variable_name = get_prev_token(list, pos);

                ASTNode *var_node =
                    create_node(variable_name.value, IDENTIFIER_NODE);
                Token eq_tok = get_next_token(list, pos);

                ASTNode *eq_node = create_node(eq_tok.value, ASSIGNMENT_NODE);
                ASTNode *expr_node = parse_expr(list, pos, 0);
                eq_node->left = var_node;
                eq_node->right = expr_node;

                Token semi_colontok = get_current_token(list, pos);

                if (semi_colontok.type == SEMICOLON_TOKEN) {
                        get_next_token(list, pos);
                }
                else {
                        perror("Syntax Error: Semi-Colon Expected");
                        exit(EXIT_FAILURE);
                }
                return eq_node;
        }
        else {
                perror("Syntax Error: Variable Name Not Found");
                exit(EXIT_FAILURE);
        }
        return NULL;
}

ASTNode *parse_statement(TokenList list, int *pos) {
        Token next_token = get_next_token(list, pos);

        if (next_token.type == KEYWORD_TOKEN) {
                return parse_var_decl(list, pos);
        }
        else if (next_token.type == IDENTIFIER_TOKEN) {
                return parse_var_decl(list, pos);
        }
        return NULL;
}

// This function serves as the entry point of the parser
ASTNode *parse(TokenList list, int *pos) {
        ASTNode *root = create_node("root", BLOCK_NODE);

        while (true) {
                ASTNode *currentLine = parse_statement(list, pos);

                if (currentLine != NULL) {
                        root->children[root->child_count] = currentLine;
                        root->child_count++;
                }
                else {
                        break;
                }
        }
        return root;
}

void print_ast(ASTNode *node, int depth) {
        if (node == NULL) {
                return;
        }

        // Print indentation based on depth
        for (int i = 0; i < depth; i++) {
                printf("  ");
        }

        // Print the node type and value
        switch (node->type) {
                case INT_NODE:
                        printf("INT: %s\n", node->value);
                        break;
                case IDENTIFIER_NODE:
                        printf("IDENTIFIER: %s\n", node->value);
                        break;
                case STRING_NODE:
                        printf("STRING: %s\n", node->value);
                        break;
                case ADDITION_NODE:
                        printf("ADDITION: %s\n", node->value);
                        break;
                case SUBTRACTION_NODE:
                        printf("SUBTRACTION: %s\n", node->value);
                        break;
                case MULTIPLICATION_NODE:
                        printf("MULTIPLICATION: %s\n", node->value);
                        break;
                case DIVISION_NODE:
                        printf("DIVISION: %s\n", node->value);
                        break;
                case ASSIGNMENT_NODE:
                        printf("ASSIGNMENT: %s\n", node->value);
                        break;
                case BLOCK_NODE:
                        printf("BLOCK: %s\n", node->value);
                        // Print all children of the block node
                        for (int i = 0; i < node->child_count; i++) {
                                print_ast(node->children[i], depth + 1);
                        }
                        return; // Return after printing children to avoid
                                // double traversal
                default:
                        printf("UNKNOWN NODE: %s (type: %d)\n", node->value,
                               node->type);
                        break;
        }

        // Recursively print left and right children for non-block nodes
        //
        print_ast(node->left, depth + 1);
        print_ast(node->right, depth + 1);
}

void print_ast_tree(ASTNode *root) {
        printf("\n=== AST Tree ===\n");
        if (root == NULL) {
                printf("(empty tree)\n");
        }
        else {
                print_ast(root, 0);
        }
        printf("===============\n");
}
