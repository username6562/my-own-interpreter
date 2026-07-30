#include "../include/ast.h"
#include <stdio.h>
#include <stdlib.h>

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

Expr *create_binary_expr(char *operator, Expr *left, Expr *right,
                         ExprType type) {
        Expr *expr = malloc(sizeof(Expr));
        expr->value = operator;
        expr->left = left;
        expr->right = right;
        expr->type = type;

        return expr;
}

Stmt *create_variable_decl_stmt(char *type, char *var_name, Expr *value,
                                bool is_declared) {
        Stmt *stmt = malloc(sizeof(Stmt));
        stmt->type = VAR_DECL_STMT;
        stmt->variable_decl.type = type;
        stmt->variable_decl.var_name = var_name;
        stmt->variable_decl.value = value;
        stmt->variable_decl.is_declared = is_declared;

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
                printf("\nStatement %d:\n", i + 1);

                if (stmt == NULL) {
                        printf("  (null statement)\n");
                        continue;
                }
                switch (stmt->type) {
                        case VAR_DECL_STMT:
                                printf("TYPE: VAR_DECL_STMT\n");
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
                }
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
                case ADDITION_OP:
                        printf("ADDITION (+)\n");
                        printf("\tLEFT: ");
                        print_expr(expr->left);
                        printf("\tRIGHT: ");
                        print_expr(expr->right);
                        break;
                case SUBTRACTION_OP:
                        printf("SUBTRACTION (-)\n");
                        printf("\tLEFT: ");
                        print_expr(expr->left);
                        printf("\tRIGHT: ");
                        print_expr(expr->right);
                        break;
                case MULTIPLICATION_OP:
                        printf("MULTIPLICATION (*)\n");
                        printf("\tLEFT: ");
                        print_expr(expr->left);
                        printf("\tRIGHT: ");
                        print_expr(expr->right);
                        break;
                case DIVISION_OP:
                        printf("DIVISION (/)\n");
                        printf("\tLEFT: ");
                        print_expr(expr->left);
                        printf("\tRIGHT: ");
                        print_expr(expr->right);
                        break;
                case ASSIGNMENT_OP:
                        printf("ASSIGNMENT (=)\n");
                        printf("\tLEFT: ");
                        print_expr(expr->left);
                        printf("\tRIGHT: ");
                        print_expr(expr->right);
                        break;
                default:
                        printf("Unknown Expr %s\n", expr->value);
        }
}
