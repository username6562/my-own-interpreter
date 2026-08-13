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
Expr *parse_expr(TokenList list, int *pos, int current_binding_power);

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
        else if (next_tok.type == L_PARENTHESIS) {
                Expr *inner_expr = parse_expr(list, pos, 0);
                Token close_paren = get_next_token(list, pos);
                if (close_paren.type == R_PARENTHESIS) {
                        return inner_expr;
                }
        }
        else if (next_tok.type == BOOL_TOKEN) {
                Expr *expr = create_bool_literal(next_tok.value);
                return expr;
        }
        else if (next_tok.type == STRING_TOKEN) {
                Expr *expr = create_string_literal(next_tok.value);
                return expr;
        }
        return NULL;
}

int get_binding_power(Token token) {
        if (token.type == BINARYOP_TOKEN) {
                if (strcmp(token.value, "+") == 0 || strcmp(token.value, "-") == 0) {
                        return 15;
                }
                else if (strcmp(token.value, "*") == 0 || strcmp(token.value, "/") == 0) {
                        return 20;
                }
                else if (strcmp(token.value, "==") == 0 || strcmp(token.value, "<=") == 0 ||
                         strcmp(token.value, ">=") == 0 || strcmp(token.value, ">") == 0 ||
                         strcmp(token.value, ">") == 0) {
                        return 10;
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
                                opNode =
                                    create_binary_expr(next_tok.value, left,
                                                       parse_expr(list, pos, next_bp), ADDITION_OP);
                        }
                        else if (strcmp(next_tok.value, "-") == 0) {
                                opNode = create_binary_expr(next_tok.value, left,
                                                            parse_expr(list, pos, next_bp),
                                                            SUBTRACTION_OP);
                        }
                        else if (strcmp(next_tok.value, "*") == 0) {
                                opNode = create_binary_expr(next_tok.value, left,
                                                            parse_expr(list, pos, next_bp),
                                                            MULTIPLICATION_OP);
                        }
                        else if (strcmp(next_tok.value, "/") == 0) {
                                opNode =
                                    create_binary_expr(next_tok.value, left,
                                                       parse_expr(list, pos, next_bp), DIVISION_OP);
                        }
                        else if (strcmp(next_tok.value, "==") == 0) {
                                opNode = create_binary_expr(next_tok.value, left,
                                                            parse_expr(list, pos, 0), EQUALS_TO_OP);
                        }
                        else if (strcmp(next_tok.value, "<=") == 0) {
                                opNode = create_binary_expr(
                                    next_tok.value, left, parse_expr(list, pos, 0), LT_OR_EQUAL_TO);
                        }
                        else if (strcmp(next_tok.value, ">=") == 0) {
                                opNode = create_binary_expr(
                                    next_tok.value, left, parse_expr(list, pos, 0), GT_OR_EQUAL_TO);
                        }
                        else if (strcmp(next_tok.value, ">") == 0) {
                                opNode =
                                    create_binary_expr(next_tok.value, left,
                                                       parse_expr(list, pos, 0), GREATER_THAN_OP);
                        }
                        else if (strcmp(next_tok.value, "<") == 0) {
                                opNode = create_binary_expr(next_tok.value, left,
                                                            parse_expr(list, pos, 0), LESS_THAN_OP);
                        }
                }
                else if (next_tok.type == EQUALS_TOKEN) {
                        opNode = create_binary_expr(next_tok.value, left,
                                                    parse_expr(list, pos, next_bp), ASSIGNMENT_OP);
                }
                else {
                        *pos = saved_pos;
                        break;
                }
                left = opNode;
        }
        return left;
}

Stmt *parse_var_decl(TokenList list, int *pos) {
        Token current_tok = get_current_token(list, pos);
        Token next_token = get_next_token(list, pos);
        if (next_token.type == IDENTIFIER_TOKEN) {
                Token equals_token = get_next_token(list, pos);
                if (equals_token.type == EQUALS_TOKEN) {

                        Expr *expr_node = parse_expr(list, pos, 0);
                        Token semi_colontok = get_next_token(list, pos);
                        if (semi_colontok.type == SEMICOLON_TOKEN) {
                                Stmt *stmt = create_variable_decl_stmt(current_tok.value,
                                                                       next_token.value, expr_node);
                                return stmt;
                        }
                        else {
                                perror("Syntax Error: Semi-Colon Expected\n");
                                exit(EXIT_FAILURE);
                        }
                }
        }
        else {
                perror("Syntax Error: Variable Name Not Found");
                exit(EXIT_FAILURE);
        }
        return NULL;
}

Stmt *parse_var_reassignment(TokenList list, int *pos) {
        Token identifier_tok = get_current_token(list, pos);
        Token next_tok = get_next_token(list, pos);

        if (next_tok.type == EQUALS_TOKEN) {
                Expr *value = parse_expr(list, pos, 0);
                Token semi_colon_tok = get_next_token(list, pos);

                if (semi_colon_tok.type == SEMICOLON_TOKEN) {
                        Stmt *stmt = create_var_assignment_stmt(identifier_tok.value, value);

                        return stmt;
                }
                else {
                        perror("Syntax Error: Semi-Colon Expected\n");
                        exit(EXIT_FAILURE);
                }
        }
        else {
                perror("Syntax Error Assignment Operator Expected\n");
                exit(EXIT_FAILURE);
        }

        return NULL;
}

Stmt *parse_if_stmt(TokenList list, int *pos) {
        Token next_tok = get_next_token(list, pos);

        if (next_tok.type != L_PARENTHESIS) {
                printf("Syntax Error Expected Open Bracket After 'if' Keyword");
                exit(EXIT_FAILURE);
        }

        if (next_tok.type == L_PARENTHESIS) {
                Expr *coondition_node = parse_expr(list, pos, 0);
                Token close_paren = get_next_token(list, pos);

                if (close_paren.type == R_PARENTHESIS) {
                        Token left_curly_tok = get_next_token(list, pos);
                        if (left_curly_tok.type == L_CURLY_BRACKETS) {

                                Stmt *stmt = create_if_stmt(coondition_node);

                                while (true) {
                                        if (get_current_token(list, pos).type == R_CURLY_BRACKET ||
                                            get_current_token(list, pos).type == EOF_TOKEN) {
                                                break;
                                        }
                                        Stmt *current_line = parse_statement(list, pos);
                                        if (current_line != NULL) {
                                                stmt->if_stmt.stmts
                                                    ->statements[stmt->if_stmt.stmts->count] =
                                                    current_line;
                                                stmt->if_stmt.stmts->count++;
                                        }
                                        else {
                                                break;
                                        }
                                }

                                Token next_token = get_next_token(list, pos);

                                if (next_token.type == ELIF_TOKEN) {
                                        Stmt *elif_stmt = parse_if_stmt(list, pos);

                                        stmt->if_stmt.elif_stmt = elif_stmt;
                                }
                                else {
                                        stmt->if_stmt.elif_stmt = NULL;
                                }

                                return stmt;
                        }
                }
        }
        return NULL;
}

Stmt *parse_statement(TokenList list, int *pos) {
        Token next_token = get_next_token(list, pos);
        if (next_token.type == KEYWORD_TOKEN) {
                return parse_var_decl(list, pos);
        }
        else if (next_token.type == IDENTIFIER_TOKEN) {
                return parse_var_reassignment(list, pos);
        }
        else if (next_token.type == IF_TOKEN) {
                return parse_if_stmt(list, pos);
        }
        else if (next_token.type == ELIF_TOKEN) {
                printf("Syntax Error Cannot Use Elif Keyword Without An If Keyowrd\n");
                exit(EXIT_FAILURE);
        }
        return NULL;
}

// This function serves as the entry point of the parser
StmtList *parse(TokenList list, int *pos) {
        StmtList *root = create_stmt_list();
        while (true) {
                if (get_current_token(list, pos).type == EOF_TOKEN) {
                        break;
                }

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
