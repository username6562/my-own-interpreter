#include "../include/ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Expr *create_int_literal(char *value) {
        Expr *expr = malloc(sizeof(Expr));
        expr->value = value;
        expr->left = NULL;
        expr->right = NULL;
        expr->type = INT_LITERAL;

        return expr;
}

Expr *create_identifier_literal(char *value) {
        Expr *expr = malloc(sizeof(Expr));
        expr->value = value;
        expr->left = NULL;
        expr->right = NULL;
        expr->type = IDENTIFIER_LITERAL;

        return expr;
}

Expr *create_bool_literal(char *value) {
        Expr *expr = malloc(sizeof(Expr));
        expr->value = value;
        expr->left = NULL;
        expr->right = NULL;
        expr->type = BOOL_LITERAL;

        return expr;
}

Expr *create_string_literal(char *value) {
        Expr *expr = malloc(sizeof(Expr));
        expr->value = value;
        expr->left = NULL;
        expr->right = NULL;
        expr->type = STRING_LITERAL;

        return expr;
}

Expr *create_binary_expr(char *operator, Expr *left, Expr *right, ExprType type) {
        Expr *expr = malloc(sizeof(Expr));
        expr->value = operator;
        expr->left = left;
        expr->right = right;
        expr->type = type;

        return expr;
}

Stmt *create_variable_decl_stmt(char *type, char *var_name, Expr *value) {
        Stmt *stmt = malloc(sizeof(Stmt));
        stmt->type = VAR_DECL_STMT;
        stmt->variable_decl.type = type;
        stmt->variable_decl.var_name = var_name;
        stmt->variable_decl.value = value;

        return stmt;
}

Stmt *create_var_assignment_stmt(char *var_name, Expr *value) {
        Stmt *stmt = malloc(sizeof(Stmt));
        stmt->type = VAR_REASSIGN_STMT;
        stmt->variable_decl.type = NULL;
        stmt->variable_decl.var_name = var_name;
        stmt->variable_decl.value = value;

        return stmt;
}

Stmt *create_if_stmt(Expr *condition) {
        Stmt *stmt = malloc(sizeof(Stmt));
        stmt->type = IF_STMT;
        stmt->if_stmt.stmts = create_stmt_list();
        stmt->if_stmt.condition = condition;

        return stmt;
}
StmtList *create_stmt_list() {
        StmtList *list = malloc(sizeof(StmtList));
        list->statements = malloc(sizeof(Stmt *) * 100);
        list->count = 0;

        return list;
}

void print_stmt_list(StmtList *list) {
        if (list == NULL) {
                printf("Empty list found");
        }

        printf("\n=== Statement List\n");
        printf("Total Statement %d\n", list->count);

        for (int i = 0; i < list->count; i++) {
                Stmt *stmt = list->statements[i];
                if (stmt->type == IF_STMT) {
                        printf("\nIf Statement %d:\n", i + 1);
                }
                else {
                        printf("\nStatement %d:\n", i + 1);

                        if (stmt == NULL) {
                                printf("  (null statement)\n");
                                continue;
                        }
                        switch (stmt->type) {
                                case VAR_DECL_STMT: {
                                        printf("TYPE: VAR_DECL_STMT\n");
                                        if (stmt->variable_decl.type != NULL)
                                                printf("VARIABLE TYPE: %s\n",
                                                       stmt->variable_decl.type);
                                        printf("VARIABLE NAME: %s\n",
                                               stmt->variable_decl.var_name);
                                        printf("VALUE\n");
                                        if (stmt->variable_decl.value != NULL) {
                                                print_expr(stmt->variable_decl.value);
                                        }
                                        else {
                                                printf("NULL VARIABLE VALUE EXPR");
                                        }
                                        break;
                                }
                                case VAR_REASSIGN_STMT: {
                                        printf("TYPE: VAR_REASSIGN_STMT");
                                        printf("VARIABLE NAME: %s\n",
                                               stmt->variable_decl.var_name);
                                        printf("VALUE\n");
                                        if (stmt->variable_decl.value != NULL) {
                                                print_expr(stmt->variable_decl.value);
                                        }
                                        else {
                                                printf("NULL VARIABLE VALUE EXPR");
                                        }
                                } break;
                                case IF_STMT:
                                        printf("TYPE: IF_STMT");
                                        printf("\n STMTS IN IF_STMT");
                                        printf("if stmt count %d",
                                               stmt->if_stmt.stmts->count);
                                        print_stmt_list(stmt->if_stmt.stmts);
                                        break;
                                default:
                                        printf("Statement Type Not Fount\n");
                        }
                }
        }
}

void print_bool_literal(char *value) {
        if (strcmp(value, "true") == 0) {
                printf("true\n");
        }
        else if (strcmp(value, "false") == 0) {
                printf("false\n");
        }
}
void print_expr(Expr *expr) {
        if (expr == NULL) {
                printf("(null expression)\n");
                return;
        }
        switch (expr->type) {
                case INT_LITERAL:
                        printf("INT: %s\n", expr->value);
                        break;
                case IDENTIFIER_LITERAL:
                        printf("IDENTIFIER: %s", expr->value);
                        break;
                case BOOL_LITERAL:
                        printf("BOOLEAN: ");
                        print_bool_literal(expr->value);
                        break;
                case STRING_LITERAL:
                        printf("STRING: %s\n", expr->value);
                        break;
                case ADDITION_OP:
                        printf("ADDITION (+)\n");
                        printf("\tLEFT (+): ");
                        print_expr(expr->left);
                        printf("\tRIGHT (+): ");
                        print_expr(expr->right);
                        break;
                case SUBTRACTION_OP:
                        printf("SUBTRACTION (-)\n");
                        printf("\tLEFT (-): ");
                        print_expr(expr->left);
                        printf("\tRIGHT (-): ");
                        print_expr(expr->right);
                        break;
                case MULTIPLICATION_OP:
                        printf("MULTIPLICATION (*)\n");
                        printf("\tLEFT (*): ");
                        print_expr(expr->left);
                        printf("\tRIGHT: (*) ");
                        print_expr(expr->right);
                        break;
                case DIVISION_OP:
                        printf("DIVISION (/)\n");
                        printf("\tLEFT (/): ");
                        print_expr(expr->left);
                        printf("\tRIGHT (/): ");
                        print_expr(expr->right);
                        break;
                case ASSIGNMENT_OP:
                        printf("ASSIGNMENT (=)\n");
                        printf("\tLEFT (=): ");
                        print_expr(expr->left);
                        printf("\tRIGHT (=)kj: ");
                        print_expr(expr->right);
                        break;
                case LESS_THAN_OP:
                        printf("LESS THAN (<)\n");
                        printf("\tLEFT (<): ");
                        print_expr(expr->left);
                        printf("\tRIGHT (<)kj: ");
                        print_expr(expr->right);
                        break;
                case GREATER_THAN_OP:
                        printf("GREATER THAN (>)\n");
                        printf("\tLEFT (>): ");
                        print_expr(expr->left);
                        printf("\tRIGHT (>)kj: ");
                        print_expr(expr->right);
                        break;

                case LT_OR_EQUAL_TO:
                        printf("LESS THAN OR EQUAL TO (<=)\n");
                        printf("\tLEFT (<=): ");
                        print_expr(expr->left);
                        printf("\tRIGHT (<=)kj: ");
                        print_expr(expr->right);
                        break;
                case GT_OR_EQUAL_TO:
                        printf("GREATER THAN OR EQUAL TO (>=)\n");
                        printf("\tLEFT (>=): ");
                        print_expr(expr->left);
                        printf("\tRIGHT (>=): ");
                        print_expr(expr->right);
                        break;
                case EQUALS_TO_OP:
                        printf("EQUAL TO (==)\n");
                        printf("\tLEFT (==): ");
                        print_expr(expr->left);
                        printf("\tRIGHT (==)kj: ");
                        print_expr(expr->right);
                        break;
                default:
                        printf("Unknown Expr %s\n", expr->value);
        }
}
