
#ifndef VARIABLE_H
#define VARIABLE_H
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
        Value value;
        bool is_declared;
} Variable;

typedef struct Scope Scope;

struct Scope {
        Variable **variable_array;
        int count;
        Scope *parent_scope;
};

extern Scope *current_scope;
void init_global_scope();
Variable *create_variable();
void set_variable(Scope *current_scope, Variable *variable);

Scope *enter_scope(Scope *current_scope);

Scope *exit_scope(Scope *current_scope);
void print_scope(Scope *current_scope);
Variable *get_variable(Scope *current_scope, char *name);
Value nil_var();
#endif
