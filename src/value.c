#include "../include/value.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Scope *create_scope() {
        Scope *scope = malloc(sizeof(Scope));
        scope->variable_array = malloc(sizeof(Variable *) * 100);
        scope->parent_scope = NULL;
        scope->count = 0;

        return scope;
}

void init_global_scope() { global_scope = create_scope(); }

Value nil_var() {
        Value val;
        val.type = NULL_VAL;

        return val;
}

void set_variable(Scope *current_scope, Variable *variable) {

        for (int i = 0; i < current_scope->count; i++) {
                Variable *current_var = current_scope->variable_array[i];
                if (strcmp(current_var->name, variable->name) == 0) {
                        if (variable->value.type != current_var->value.type) {
                                printf("Run Time Error: Cannot Reassign "
                                       "Variable %s To Another Type",
                                       variable->name);
                                exit(EXIT_FAILURE);
                        }
                        if (variable->value.type == STRING_VAL) {
                                current_var->value.as.string_val =
                                    strdup(variable->value.as.string_val);
                                return;
                        }

                        current_var->value = variable->value;
                }
        }

        if (variable->value.type == STRING_VAL) {
                variable->value.as.string_val = strdup(variable->value.as.string_val);
        }
        current_scope->variable_array[current_scope->count] = variable;
        current_scope->count++;
}

Variable *get_variable(Scope *current_scope, char *name) {
        while (current_scope != NULL) {
                for (int i = 0; i < current_scope->count; i++) {
                        Variable *current_var = current_scope->variable_array[i];
                        if (strcmp(current_var->name, name) == 0) {
                                return current_var;
                        }
                }
                current_scope = current_scope->parent_scope;
        }
        return NULL;
}

Variable *create_variable() {
        Variable *variable = malloc(sizeof(Variable));
        return variable;
}

/*
 * Usage: Gets the current scope of a codebase and makes a new child node
 */
Scope *enter_scope(Scope *current_scope) {
        Scope *new_scope = create_scope();
        new_scope->parent_scope = current_scope;

        return new_scope;
}

Scope *exit_scope(Scope *current_scope) {
        Scope *new_scope = create_scope();

        if (current_scope->parent_scope == NULL) {
                return current_scope;
        }
        new_scope = current_scope->parent_scope;
        return new_scope;
}

void print_scope(Scope *current_scope) {
        for (int i = 0; i < current_scope->count; i++) {
                Variable *current_var = current_scope->variable_array[i];
                if (current_var->value.type == INT_VAL) {

                        printf("variable name  %s | value %d \n",
                               current_scope->variable_array[i]->name,
                               current_scope->variable_array[i]->value.as.int_val);
                }
                else if (current_var->value.type == STRING_VAL) {

                        printf("variable name  %s value %s \n",
                               current_scope->variable_array[i]->name,
                               current_scope->variable_array[i]->value.as.string_val);
                }
                else {
                        printf("variable name  %s value %d \n",
                               current_scope->variable_array[i]->name,
                               current_scope->variable_array[i]->value.as.bool_val);
                }
        }
}
FunctionList *create_function_list() {
        FunctionList *list = malloc(sizeof(FunctionList));
        list->count = 0;
        list->capacity = 100;
        list->functions = malloc(sizeof(Function *) * list->capacity);

        return list;
}
void add_function(Function *function) {
        if (list->count >= list->capacity) {
                list->capacity *= 2;
                list->functions = realloc(list->functions, sizeof(Function *) * list->capacity);
                list->functions[list->count++] = function;
        }
        list->functions[list->count++] = function;
}

void init_function_list() { list = create_function_list(); }

Function *get_function(char *name) {
        for (int i = 0; i < list->capacity; i++) {
                if (strcmp(list->functions[i]->name, name) == 0) {
                        return list->functions[i];
                }
        }
        return NULL;
}
