#include "../include/interpreter.h"
#include "../include/value.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
Scope *global_scope = NULL;

int eval_int_expr(Expr *expr) {
        switch (expr->type) {
                case INT_LITERAL:
                        return atoi(expr->value);
                        break;
                case ADDITION_OP:
                        return eval_int_expr(expr->left) +
                               eval_int_expr(expr->right);
                        break;
                case SUBTRACTION_OP:
                        return eval_int_expr(expr->left) -
                               eval_int_expr(expr->right);
                        break;
                case MULTIPLICATION_OP:
                        return eval_int_expr(expr->left) *
                               eval_int_expr(expr->right);
                        break;
                case DIVISION_OP:
                        return eval_int_expr(expr->left) /
                               eval_int_expr(expr->right);
                        break;

                default:
                        perror("Run Time Error Operator Not Supported By "
                               "Interpreter");
                        exit(EXIT_FAILURE);
        }
        return 0;
}

bool eval_bool_literal(Expr *expr) {
        if (strcmp(expr->value, "true")) {
                return true;
        }
        else if (strcmp(expr->value, "false")) {
                return false;
        }
        return 0;
}

bool eval_bool_expr(Expr *expr) {
        switch (expr->type) {
                case BOOL_LITERAL:
                        return eval_bool_literal(expr);
                        break;
                case LESS_THAN_OP:
                        break;
                default:
                        perror("Run Time Error Operator Could Not Be Found");
                        exit(EXIT_FAILURE);
        }
        return 0;
}

Variable eval_stmt_decl(Stmt stmt, Scope *current_scope) {
        switch (stmt.type) {
                case VAR_DECL_STMT: {
                        if (strcmp(stmt.variable_decl.type, "int") == 0) {
                                Variable variable;
                                variable.name = stmt.variable_decl.var_name;
                                variable.value.type = INT_VAL;
                                variable.value.as.int_val =
                                    eval_int_expr(stmt.variable_decl.value);
                                set_variable(current_scope, variable);
                        }
                } break;
                default:
                        perror("Run Time Error: Statement Type Couldn't Be "
                               "Gotten");
        }

        return null_var();
}
