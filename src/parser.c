#include "../include/parser.h"
#include "../include/ast.h"
#include "../include/lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
Expr *parse_primary(TokenList list, int *pos) {
        Token next_tok = get_next_token(list, pos);

        if (next_tok.type == INT_TOKEN) {
                Expr *node = create_int_literal(next_tok.value);
                return node;
        }
        else if (next_tok.type == IDENTIFIER_TOKEN) {
                Expr *node = create_identifier_literal(next_tok.value);
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
Expr *parse_expr(TokenList list, int *pos, int current_binding_power) {
        Expr *left = parse_primary(list, pos);

        while (true) {
                int saved_pos = *pos;
                Token next_tok = get_next_token(list, pos);
                int next_bp = get_binding_power(next_tok);

                if (current_binding_power >= next_bp) {
                        *pos = saved_pos;
                        break;
                }
                Expr *opNode;
                if (next_tok.type == BINARYOP_TOKEN) {
                        if (strcmp(next_tok.value, "+") == 0) {
                                opNode = create_binary_expr(
                                    next_tok.value, left,
                                    parse_expr(list, pos, next_bp),
                                    ADDITION_OP);
                        }
                        else if (strcmp(next_tok.value, "-") == 0) {
                                opNode = create_binary_expr(
                                    next_tok.value, left,
                                    parse_expr(list, pos, next_bp),
                                    SUBTRACTION_OP);
                        }
                        else if (strcmp(next_tok.value, "*") == 0) {
                                opNode = create_binary_expr(
                                    next_tok.value, left,
                                    parse_expr(list, pos, next_bp),
                                    MULTIPLICATION_OP);
                        }
                        else if (strcmp(next_tok.value, "/") == 0) {
                                opNode = create_binary_expr(
                                    next_tok.value, left,
                                    parse_expr(list, pos, next_bp),
                                    DIVISION_OP);
                        }
                }
                else if (next_tok.type == EQUALS_TOKEN) {
                        opNode = create_binary_expr(
                            next_tok.value, left,
                            parse_expr(list, pos, next_bp), ASSIGNMENT_OP);
                }
                else {
                        break;
                }
                left = opNode;
        }

        return left;
}

// Inputs a stream of token and makes a Tree Node of Variable Assignments and
// Declaration
Stmt *parse_var_decl(TokenList list, int *pos) {
        Token var_type = get_current_token(list, pos);
        Token next_token = get_next_token(list, pos);

        //  Makes an ASTNode for when a variable is  declared
        if (next_token.type == IDENTIFIER_TOKEN) {
                Token equals_token = get_next_token(list, pos);

                if (equals_token.type == EQUALS_TOKEN) {
                        Expr *expr_node = parse_expr(list, pos, 0);

                        Stmt *stmt = create_variable_decl_stmt(
                            var_type.value, next_token.value, expr_node, true);

                        Token semi_colontok = get_next_token(list, pos);
                        if (semi_colontok.type == SEMICOLON_TOKEN) {
                                get_current_token(list, pos);
                        }
                        else {
                                perror("Syntax Error: Semi-Colon Expected\n");
                                exit(EXIT_FAILURE);
                        }
                        return stmt;
                }
        }
        else {
                perror("Syntax Error: Variable Name Not Found");
                exit(EXIT_FAILURE);
        }
        return NULL;
}

Stmt *parse_statement(TokenList list, int *pos) {
        Token next_token = get_next_token(list, pos);

        if (next_token.type == KEYWORD_TOKEN) {
                return parse_var_decl(list, pos);
        }
        return NULL;
}

// This function serves as the entry point of the parser
StmtList *parse(TokenList list, int *pos) {
        StmtList *root = create_stmt_list();

        while (true) {
                Stmt *currentLine = parse_statement(list, pos);
                if (currentLine != NULL) {
                        root->statements[root->count] = currentLine;
                        root->count++;
                }
                else {
                        break;
                }
        }
        return root;
}
