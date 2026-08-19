#include "../include/interpreter.h"
#include "../include/value.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
Scope *current_scope = NULL;

bool eval_bool_literal(Expr *expr) {
        if (strcmp(expr->value, "true") == 0) {
                return true;
        }
        else if (strcmp(expr->value, "false") == 0) {
                return false;
        }
        return 0;
}

bool eval_comparison_expr(ExprType type, Value a, Value b) {
        if (a.type != b.type)
                return 0;

        switch (type) {
                case LESS_THAN_OP:
                        return a.as.int_val < b.as.int_val;
                        break;
                case GREATER_THAN_OP:
                        return a.as.int_val > b.as.int_val;
                        break;
                case LT_OR_EQUAL_TO:
                        return a.as.int_val <= b.as.int_val;
                        break;
                case GT_OR_EQUAL_TO:
                        return a.as.int_val >= b.as.int_val;
                        break;
                case EQUALS_TO_OP:
                        switch (a.type) {
                                case INT_VAL:
                                        return a.as.int_val == b.as.int_val;
                                        break;
                                case BOOL_VAL:
                                        return a.as.bool_val == b.as.bool_val;
                                        break;
                                default:
                                        perror("Value type is not Supported By "
                                               "The EQUALS TO OPERATOR");
                                        exit(EXIT_FAILURE);
                        }
                        break;
                default:
                        perror("INVALID COMPARISON OPERATOR");
                        exit(EXIT_FAILURE);
        }
}

Value eval_expr(Scope *current_scope, Expr *expr) {
        switch (expr->type) {
                case INT_LITERAL: {
                        Value value;
                        value.type = INT_VAL;
                        value.as.int_val = atoi(expr->value);
                        return value;
                } break;
                case STRING_LITERAL: {
                        Value value;
                        value.type = STRING_VAL;
                        value.as.string_val = expr->value;

                        return value;
                } break;
                case BOOL_LITERAL: {
                        Value value;
                        value.type = BOOL_VAL;
                        value.as.bool_val = eval_bool_literal(expr);

                        return value;
                } break;
                case IDENTIFIER_LITERAL: {
                        Value value = get_variable(current_scope, expr->value)->value;
                        return value;
                }
                case ADDITION_OP: {
                        Value left_val = eval_expr(current_scope, expr->left);
                        Value right_val = eval_expr(current_scope, expr->right);
                        Value result;
                        result.type = INT_VAL;
                        result.as.int_val = left_val.as.int_val + right_val.as.int_val;

                        return result;
                } break;
                case SUBTRACTION_OP: {
                        Value left_val = eval_expr(current_scope, expr->left);
                        Value right_val = eval_expr(current_scope, expr->right);
                        Value result;
                        result.type = INT_VAL;
                        result.as.int_val = left_val.as.int_val - right_val.as.int_val;
                        return result;
                }

                break;
                case MULTIPLICATION_OP: {
                        Value left_val = eval_expr(current_scope, expr->left);
                        Value right_val = eval_expr(current_scope, expr->right);
                        Value result;
                        result.type = INT_VAL;
                        result.as.int_val = left_val.as.int_val * right_val.as.int_val;
                        return result;
                } break;
                case DIVISION_OP: {
                        Value left_val = eval_expr(current_scope, expr->left);
                        Value right_val = eval_expr(current_scope, expr->right);
                        Value result;
                        result.type = INT_VAL;
                        result.as.int_val = left_val.as.int_val / right_val.as.int_val;
                        return result;
                } break;
                case LESS_THAN_OP: {
                        Value left_val = eval_expr(current_scope, expr->left);
                        Value right_val = eval_expr(current_scope, expr->right);
                        Value result;
                        result.type = BOOL_VAL;
                        result.as.bool_val =
                            eval_comparison_expr(LESS_THAN_OP, left_val, right_val);
                        return result;
                } break;
                case GREATER_THAN_OP: {
                        Value left_val = eval_expr(current_scope, expr->left);
                        Value right_val = eval_expr(current_scope, expr->right);
                        Value result;
                        result.type = BOOL_VAL;
                        result.as.bool_val =
                            eval_comparison_expr(GREATER_THAN_OP, left_val, right_val);
                        return result;
                } break;
                case LT_OR_EQUAL_TO: {
                        Value left_val = eval_expr(current_scope, expr->left);
                        Value right_val = eval_expr(current_scope, expr->right);
                        Value result;
                        result.type = BOOL_VAL;
                        result.as.bool_val =
                            eval_comparison_expr(LT_OR_EQUAL_TO, left_val, right_val);
                        return result;
                } break;
                case GT_OR_EQUAL_TO: {
                        Value left_val = eval_expr(current_scope, expr->left);
                        Value right_val = eval_expr(current_scope, expr->right);
                        Value result;
                        result.type = BOOL_VAL;
                        result.as.bool_val =
                            eval_comparison_expr(GT_OR_EQUAL_TO, left_val, right_val);
                        return result;
                } break;
                case EQUALS_TO_OP: {
                        Value left_val = eval_expr(current_scope, expr->left);
                        Value right_val = eval_expr(current_scope, expr->right);
                        Value result;
                        result.type = BOOL_VAL;
                        result.as.bool_val =
                            eval_comparison_expr(EQUALS_TO_OP, left_val, right_val);
                        return result;
                } break;
                default:
                        perror("Run Time Error Operator Not Supported By "
                               "Interpreter\n");
                        exit(EXIT_FAILURE);
        }
        return nil_var();
}

Variable *eval_stmts(Stmt *stmt, Scope *current_scope) {
        printf("Processing statement type: %d\n", stmt->type);
        switch (stmt->type) {
                case VAR_DECL_STMT: {
                        if (strcmp(stmt->variable_decl.type, "int") == 0) {
                                Variable *variable = create_variable();
                                variable->name = stmt->variable_decl.var_name;
                                variable->value.type = INT_VAL;
                                Value value = eval_expr(current_scope, stmt->variable_decl.value);
                                if (variable->value.type == value.type) {
                                        set_variable(current_scope, variable);
                                        variable->value = value;
                                        return variable;
                                }
                                else {
                                        perror("Assigning Variable To Wrong Type");
                                        exit(1);
                                }
                        }
                        else if (strcmp(stmt->variable_decl.type, "bool") == 0) {
                                Variable *variable = create_variable();
                                variable->name = stmt->variable_decl.var_name;
                                variable->value.type = BOOL_VAL;
                                Value value = eval_expr(current_scope, stmt->variable_decl.value);
                                if (variable->value.type == value.type) {
                                        variable->value = value;
                                        set_variable(current_scope, variable);
                                        variable->value = value;
                                        return variable;
                                }
                                else {
                                        perror("Assigning Variable To Wrong Type");
                                        exit(1);
                                }
                        }
                        else if (strcmp(stmt->variable_decl.type, "string") == 0) {

                                Variable *variable = create_variable();
                                variable->name = stmt->variable_decl.var_name;
                                variable->value.type = STRING_VAL;
                                Value value = eval_expr(current_scope, stmt->variable_decl.value);
                                if (variable->value.type == value.type) {
                                        variable->value = value;
                                        set_variable(current_scope, variable);
                                        variable->value = value;
                                        return variable;
                                }
                                else {
                                        perror("Assigning Variable To Wrong Type");
                                        exit(1);
                                }
                        }
                } break;
                case VAR_REASSIGN_STMT: {
                        Variable *variable =
                            get_variable(current_scope, stmt->variable_decl.var_name);
                        if (variable != NULL) {
                                Value val = eval_expr(current_scope, stmt->variable_decl.value);
                                variable->value = val;
                                set_variable(current_scope, variable);
                                return variable;
                        }

                } break;
                case IF_STMT: {
                        Value if_condition = eval_expr(current_scope, stmt->if_stmt.condition);
                        Stmt *elif_stmt = stmt->if_stmt.elif_stmt;

                        if (if_condition.type != BOOL_VAL) {
                                printf("Type Error Expected Boolean Type Not Found In "
                                       "Condition\n");
                                exit(EXIT_FAILURE);
                        }

                        if (if_condition.as.bool_val == true) {
                                Scope *new_scope = enter_scope(current_scope);
                                for (int i = 0; i < stmt->if_stmt.stmts->count; i++) {
                                        Stmt *current_if_stmt = stmt->if_stmt.stmts->statements[i];

                                        eval_stmts(current_if_stmt, new_scope);
                                }
                                print_scope(new_scope);
                                exit_scope(new_scope);
                        }

                        /*
                         *  If if_condition is not true and elif_stmt is not null and elif_condition
                         * is not false
                         */
                        else if (elif_stmt != NULL) {
                                Scope *elif_scope = enter_scope(current_scope);
                                eval_stmts(elif_stmt, elif_scope);

                                exit_scope(elif_scope);
                        }

                } break;
                case ELSE_STMT: {
                        for (int i = 0; i < stmt->else_stmt.stmts->count; i++) {
                                Stmt *current_stmt = stmt->else_stmt.stmts->statements[i];

                                eval_stmts(current_stmt, current_scope);
                        }
                } break;
                case WHILE_STMT: {
                        Value while_condition =
                            eval_expr(current_scope, stmt->while_stmt.condition);
                        int i = 0;

                        while (while_condition.as.bool_val == true) {
                                Stmt *current_stmt = stmt->while_stmt.stmts->statements[i++];
                                eval_stmts(current_stmt, current_scope);
                        }
                        printf("loop ran %d time", i);

                } break;
                default:
                        printf("Warning: Unknown or unhandled statement type %d\n", stmt->type);
                        return NULL;
                        break;
        }
        return NULL;
}
