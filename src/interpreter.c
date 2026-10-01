#include "../include/interpreter.h"
#include "../include/value.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
Scope *global_scope = NULL;
FunctionList *list = NULL;
bool has_returned_val;
Value func_return_value;

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
bool match_type(char *paramtype, ValueType valuetype) {
        if (strcmp(paramtype, "int") == 0 && valuetype == INT_VAL) {
                return true;
        }
        else if (strcmp(paramtype, "bool") == 0 && valuetype == BOOL_VAL) {
                return true;
        }
        else if (strcmp(paramtype, "string") == 0 && valuetype == STRING_VAL) {
                return true;
        }
        return false;
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
                        Variable *var = get_variable(current_scope, expr->value);

                        if (var == NULL) {
                                printf("Failed To Find Variable %s\n", var->name);
                                exit(EXIT_FAILURE);
                        }
                        Value value = var->value;
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
                case FUNC_CALL_EXPR: {
                        Function *function = get_function(expr->func_call.name);

                        if (function == NULL) {
                                printf("Run Time Error: Could Not Find Function %s",
                                       expr->func_call.name);
                        }
                        if (expr->func_call.args->count != function->params->count) {
                                printf("Expected %d arguments but only found %d",
                                       function->params->count, expr->func_call.args->count);
                        }

                        Scope *func_scope = enter_scope(current_scope);
                        while (expr->func_call.args != NULL && function->params != NULL) {
                                Argument *arg = expr->func_call.args;
                                Parameter *param = function->params;

                                Value arg_val = eval_expr(current_scope, arg->expr);

                                if (match_type(param->type, arg_val.type)) {
                                        Variable *var = create_variable();

                                        var->name = param->name;
                                        var->value = arg_val;

                                        set_variable(func_scope, var);
                                }

                                arg = arg->next;
                                param = param->next;
                        }

                        for (int i = 0; i < function->body->count && has_returned_val == false;
                             i++) {
                                eval_stmts(function->body->statements[i], func_scope);
                        }

                        if (has_returned_val) {
                                printf("return value is %d\n", func_return_value.as.int_val);
                                return func_return_value;
                        }
                        else {
                                return nil_var();
                        }

                        has_returned_val = false;
                        exit_scope(func_scope);
                } break;
                default:
                        perror("Run Time Error Operator Not Supported By "
                               "Interpreter\n");
                        exit(EXIT_FAILURE);
        }
        return nil_var();
}

Variable *eval_stmts(Stmt *stmt, Scope *current_scope) {
        // printf("\nProcessing statement type: %d\n", stmt->type);
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
                case FOR_STMT: {
                        Value count = eval_expr(current_scope, stmt->for_stmt.count);

                        if (count.type != INT_VAL) {
                                printf("Invalid Expression Used In The For Loop");
                                exit(EXIT_FAILURE);
                        }

                        Scope *for_scope = enter_scope(current_scope);

                        /*
                         * The Inner Loop is to Run through All The Statements In the For Loop
                         * and Evaluate them
                         * The Outer For Loop Repeats the Evaluation The Number Of Times In the
                         * Expression
                         */

                        for (int i = 0; i < count.as.int_val; i++) {
                                for (int j = 0; j < stmt->for_stmt.stmts->count; i++) {
                                        Stmt *current_stmt = stmt->for_stmt.stmts->statements[j];

                                        eval_stmts(current_stmt, for_scope);
                                }
                        }
                        exit_scope(for_scope);
                } break;
                case ELSE_STMT: {
                        for (int i = 0; i < stmt->else_stmt.stmts->count; i++) {
                                Stmt *current_stmt = stmt->else_stmt.stmts->statements[i];

                                eval_stmts(current_stmt, current_scope);
                        }
                } break;
                case WHILE_STMT: {
                        Expr *while_cond_expr = stmt->while_stmt.condition;

                        StmtList *while_stmt = stmt->while_stmt.stmts;
                        int i = 0;
                        while (true) {
                                Value while_condition = eval_expr(current_scope, while_cond_expr);

                                if (while_condition.as.bool_val == false) {
                                        break;
                                }
                                for (int i = 0; i < while_stmt->count; i++) {
                                        Stmt *current_stmt = while_stmt->statements[i];
                                        eval_stmts(current_stmt, current_scope);
                                }
                                i++;
                        }
                        printf("loop ran %d times\n", i);
                } break;
                case FUNC_DECL_STMT: {
                        Function *func = malloc(sizeof(Function));
                        func->name = stmt->func_decl.func_name;
                        func->body = stmt->func_decl.stmts;
                        func->params = stmt->func_decl.params;

                        add_function(func);

                } break;
                case RETURN_STMT: {
                        func_return_value = eval_expr(current_scope, stmt->return_stmt.ret_value);
                        has_returned_val = true;
                } break;
                default:
                        // printf("\nWarning: Unknown or unhandled statement type %d\n",
                        // stmt->type);
                        return NULL;
                        break;
        }
        return NULL;
}
