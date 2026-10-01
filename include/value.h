
#ifndef VARIABLE_H
#define VARIABLE_H
#include "ast.h"
#include <stdbool.h>
typedef enum {
        INT_VAL,
        BOOL_VAL,
        STRING_VAL,
        NULL_VAL
} ValueType;

typedef struct {
        ValueType type;
        union {
                int int_val;
                bool bool_val;
                char *string_val;
        } as;
} Value;

typedef struct {
        char *name;
        Parameter *params;
        StmtList *body;
} Function;

typedef struct FunctionList {
        Function **functions;
        int count;
        int capacity;
} FunctionList;

typedef struct {
        char *name;
        Value value;
        bool is_declared;
} Variable;

typedef struct Scope Scope;

struct Scope {
        Variable **variable_array;
        int count;
        Scope *parent_scope;
};

extern Scope *global_scope;
void init_global_scope();
Variable *create_variable();
void set_variable(Scope *current_scope, Variable *variable);
extern FunctionList *list;
void init_function_list();

FunctionList *create_function_list();
void add_function(Function *function);
Function *get_function(char *name);

Scope *enter_scope(Scope *current_scope);

Scope *exit_scope(Scope *current_scope);
void print_scope(Scope *current_scope);
Variable *get_variable(Scope *current_scope, char *name);
Value nil_var();
#endif
