#include "../include/parser.h"
#include "../include/ast.h"
#include "../include/lexer.h"
#include <corecrt.h>
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

Argument *parse_args(TokenList list, int *pos);
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

void parse_block(StmtList *stmts, TokenList list, int *pos, bool is_in_function) {
        while (true) {
                if (get_current_token(list, pos).type == R_CURLY_BRACKET) {
                        break;
                }

                Stmt *current_line = parse_statement(list, pos, is_in_function);

                if (current_line != NULL) {
                        stmts->statements[stmts->count] = current_line;
                        stmts->count++;
                }
                else {
                        break;
                }
        }
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
                if (get_next_token(list, pos).type == L_PARENTHESIS) {
                        Argument *args = parse_args(list, pos);

                        Token close_paren = get_next_token(list, pos);
                        printf("POS %d\n", *pos);
                        printf("token type %d \n", close_paren.type);
                        if (close_paren.type == R_PARENTHESIS) {
                                Expr *func_expr = create_funct_call(next_tok.value, args);

                                return func_expr;
                        }
                        else {
                                printf("Syntax Error Right Parenthesis ')' Not Found\n");
                        }
                }
                else {
                        get_prev_token(list, pos);
                        Expr *node = create_identifier_literal(next_tok.value);
                        return node;
                }
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
                         strcmp(token.value, ">=") == 0 ||
                         strcmp(token.value, "<") == 0 || // ← ADD THIS
                         strcmp(token.value, ">") == 0) { // ← Keep this
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
                Expr *op_node;
                if (next_tok.type == BINARYOP_TOKEN) {
                        if (strcmp(next_tok.value, "+") == 0) {
                                op_node =
                                    create_binary_expr(next_tok.value, left,
                                                       parse_expr(list, pos, next_bp), ADDITION_OP);
                        }
                        else if (strcmp(next_tok.value, "-") == 0) {
                                op_node = create_binary_expr(next_tok.value, left,
                                                             parse_expr(list, pos, next_bp),
                                                             SUBTRACTION_OP);
                        }
                        else if (strcmp(next_tok.value, "*") == 0) {
                                op_node = create_binary_expr(next_tok.value, left,
                                                             parse_expr(list, pos, next_bp),
                                                             MULTIPLICATION_OP);
                        }
                        else if (strcmp(next_tok.value, "/") == 0) {
                                op_node =
                                    create_binary_expr(next_tok.value, left,
                                                       parse_expr(list, pos, next_bp), DIVISION_OP);
                        }
                        else if (strcmp(next_tok.value, "==") == 0) {
                                op_node = create_binary_expr(next_tok.value, left,
                                                             parse_expr(list, pos, next_bp),
                                                             EQUALS_TO_OP);
                        }
                        else if (strcmp(next_tok.value, "<=") == 0) {
                                op_node = create_binary_expr(next_tok.value, left,
                                                             parse_expr(list, pos, next_bp),
                                                             LT_OR_EQUAL_TO);
                        }
                        else if (strcmp(next_tok.value, ">=") == 0) {
                                op_node = create_binary_expr(next_tok.value, left,
                                                             parse_expr(list, pos, next_bp),
                                                             GT_OR_EQUAL_TO);
                        }
                        else if (strcmp(next_tok.value, ">") == 0) {
                                op_node = create_binary_expr(next_tok.value, left,
                                                             parse_expr(list, pos, next_bp),
                                                             GREATER_THAN_OP);
                        }
                        else if (strcmp(next_tok.value, "<") == 0) {
                                op_node = create_binary_expr(next_tok.value, left,
                                                             parse_expr(list, pos, next_bp),
                                                             LESS_THAN_OP);
                        }
                }
                else if (next_tok.type == EQUALS_TOKEN) {
                        op_node = create_binary_expr(next_tok.value, left,
                                                     parse_expr(list, pos, next_bp), ASSIGNMENT_OP);
                }
                else {
                        *pos = saved_pos;
                        break;
                }
                left = op_node;
        }
        return left;
}

Stmt *parse_var_decl(TokenList list, int *pos) {
        Token current_tok = get_current_token(list, pos);
        Token next_token = get_next_token(list, pos);
        if (next_token.type == IDENTIFIER_TOKEN) {
                Token equals_token = get_next_token(list, pos);
                if (equals_token.type == EQUALS_TOKEN) {
                        printf("\n\n The expression is below??\n");
                        Expr *expr_node = parse_expr(list, pos, 0);
                        Token semi_colontok = get_next_token(list, pos);
                        if (semi_colontok.type == SEMICOLON_TOKEN) {
                                Stmt *stmt = create_variable_decl_stmt(current_tok.value,
                                                                       next_token.value, expr_node);
                                return stmt;
                        }
                        else {
                                printf("Syntax Error: Semi-Colon Expected\n");
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

Stmt *parse_else_stmt(TokenList list, int *pos) {
        Token next_tok = get_next_token(list, pos);

        if (next_tok.type == L_CURLY_BRACKETS) {
                Stmt *stmt = create_else_stmt();
                while (true) {
                        if (get_current_token(list, pos).type == R_CURLY_BRACKET) {
                                break;
                        }
                        Stmt *current_line = parse_statement(list, pos, false);

                        if (current_line != NULL) {
                                stmt->else_stmt.stmts->statements[stmt->else_stmt.stmts->count] =
                                    current_line;
                                stmt->else_stmt.stmts->count++;
                        }
                        else {
                                break;
                        }
                }
                return stmt;
        }

        return NULL;
}

Stmt *parse_cond_stmt(TokenList list, int *pos) {
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
                                        Stmt *current_line = parse_statement(list, pos, false);
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
                                        Stmt *elif_stmt = parse_cond_stmt(list, pos);

                                        stmt->if_stmt.elif_stmt = elif_stmt;
                                }
                                else if (next_token.type == ELSE_TOKEN) {
                                        Stmt *elif_stmt = parse_else_stmt(list, pos);
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

Stmt *parse_while_stmt(TokenList list, int *pos) {
        Token next_tok = get_next_token(list, pos);

        if (next_tok.type == L_PARENTHESIS) {
                Expr *condition_node = parse_expr(list, pos, 0);
                print_expr(condition_node);

                Token right_paren = get_next_token(list, pos);
                if (right_paren.type == R_PARENTHESIS) {
                        Token left_curly_tok = get_next_token(list, pos);

                        if (left_curly_tok.type == L_CURLY_BRACKETS) {
                                Stmt *stmt = create_while_stmt(condition_node);

                                parse_block(stmt->while_stmt.stmts, list, pos, false);
                                return stmt;
                        }
                }
        }
        return NULL;
}

Stmt *parse_for_stmt(TokenList list, int *pos) {
        Token open_paren = get_next_token(list, pos);

        if (open_paren.type == L_PARENTHESIS) {
                Expr *count = parse_expr(list, pos, 0);
                Token close_paren = get_next_token(list, pos);

                if (close_paren.type == R_PARENTHESIS) {
                        Token left_curly_tok = get_next_token(list, pos);

                        if (left_curly_tok.type == L_CURLY_BRACKETS) {
                                Stmt *stmt = create_for_stmt(count);
                                parse_block(stmt->for_stmt.stmts, list, pos, false);

                                return stmt;
                        }
                }
        }
        return NULL;
}

Parameter *parse_parameter(TokenList list, int *pos) {
        Parameter *current;
        Parameter *head = NULL;
        int count = 0;

        while (get_current_token(list, pos).type != R_PARENTHESIS) {

                Token param_type = get_next_token(list, pos);
                Token param_name = get_next_token(list, pos);

                Parameter *p = malloc(sizeof(Parameter));
                p->name = param_name.value;
                p->type = param_type.value;
                p->count = ++count;
                p->next = NULL;
                if (head == NULL) {
                        head = p;
                        current = p;
                }
                else {
                        current->next = p;
                        current = p;
                }

                if (get_current_token(list, pos).type == R_PARENTHESIS) {
                        break;
                }
                get_next_token(list, pos);
        }
        if (head != NULL) {
                head->count = count;
        }
        return head;
}

Stmt *parse_funct_decl(TokenList list, int *pos) {
        Token func_name = get_next_token(list, pos);

        if (get_next_token(list, pos).type == L_PARENTHESIS) {
                Parameter *params = parse_parameter(list, pos);

                if (get_next_token(list, pos).type == L_CURLY_BRACKETS) {
                        Stmt *stmt = create_func_decl_stmt(func_name.value, params);
                        parse_block(stmt->func_decl.stmts, list, pos, true);

                        return stmt;
                }
        }

        return NULL;
}

Argument *parse_args(TokenList list, int *pos) {
        Argument *head = NULL;
        Argument *current;
        int count = 0;

        if (get_current_token(list, pos).type == R_PARENTHESIS) {
                return NULL;
        }
        while (get_current_token(list, pos).type != R_PARENTHESIS) {
                Argument *a = malloc(sizeof(Argument));
                a->next = NULL;
                a->expr = parse_expr(list, pos, 0);

                if (a->expr == NULL) {
                        printf("Invalid Expression Used in Argument\n");
                        exit(1);
                }
                print_expr(a->expr);

                if (head == NULL) {
                        head = a;
                        current = a;
                }
                else {
                        current->next = a;
                        current = a;
                }

                count++;

                Token tok = get_next_token(list, pos);

                if (tok.type == COMMA_TOKEN) {
                        continue;
                }
                if (tok.type == R_PARENTHESIS) {
                        *pos -= 1;
                        break;
                }
                printf("Syntax Error: expected ',' or ')' in argument list, got '%s'\n", tok.value);
                exit(EXIT_FAILURE);
        }

        printf("COUNT %d\n", count);
        if (head != NULL) {
                head->count = count;
        }

        return head;
}

Stmt *parse_ret_stmt(TokenList list, int *pos) {
        Stmt *stmt = malloc(sizeof(Stmt));
        stmt->type = RETURN_STMT;
        Expr *return_value = parse_expr(list, pos, 0);
        stmt->return_stmt.ret_value = return_value;

        if (get_next_token(list, pos).type != SEMICOLON_TOKEN) {
                printf("Syntax Error: Semi-Colon Not Found");
                exit(1);
        }

        if (return_value != NULL) {
                return stmt;
        }
        return NULL;
}

Stmt *parse_statement(TokenList list, int *pos, bool is_in_function) {
        Token next_token = get_next_token(list, pos);
        switch (next_token.type) {
                case KEYWORD_TOKEN: {
                        return parse_var_decl(list, pos);
                } break;
                case IDENTIFIER_TOKEN: {
                        return parse_var_reassignment(list, pos);
                }
                case IF_TOKEN: {
                        return parse_cond_stmt(list, pos);
                } break;
                case ELIF_TOKEN: {
                        printf("Syntax Error Cannot Use Elif Keyword Without An If Keyowrd\n");
                        exit(EXIT_FAILURE);
                } break;
                case WHILE_TOKEN: {
                        return parse_while_stmt(list, pos);
                } break;
                case FOR_TOKEN: {
                        return parse_for_stmt(list, pos);
                } break;
                case FUNC_DECL_TOKEN: {
                        return parse_funct_decl(list, pos);
                } break;
                case RETURN_TOKEN: {
                        if (is_in_function) {
                                return parse_ret_stmt(list, pos);
                        }
                        return NULL;
                }
                default:
                        break;
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

                Stmt *currentLine = parse_statement(list, pos, 0);
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
