#ifndef AST_H
#define AST_H

#include <stdbool.h>

typedef enum {
        INT_LITERAL,
        IDENTIFIER_LITERAL,
        BOOL_LITERAL,
        STRING_LITERAL,
        ADDITION_OP,
        SUBTRACTION_OP,
        MULTIPLICATION_OP,
        DIVISION_OP,
        ASSIGNMENT_OP,
        LT_OR_EQUAL_TO, // Less Than Or Equal To
        GT_OR_EQUAL_TO, // Greater Than Or Equal To
        LESS_THAN_OP,
        GREATER_THAN_OP,
        EQUALS_TO_OP,
        FUNC_CALL_EXPR
} ExprType;

typedef struct Expr Expr;
typedef struct Stmt Stmt;
typedef struct StmtList StmtList;
typedef struct Parameter Parameter;
typedef struct Argument Argument;
struct Expr {
        char *value;
        ExprType type;
        Expr *left;
        Expr *right;
        union {
                struct {
                        Argument *args;
                        char *name;
                } func_call;
        };
};

struct Parameter {
        char *type;
        char *name;
        int count;
        Parameter *next;
};

struct Argument {
        Expr *expr;
        int count;
        Argument *next;
};

typedef enum {
        VAR_DECL_STMT,
        VAR_REASSIGN_STMT,
        IF_STMT,
        ELSE_STMT,
        WHILE_STMT,
        FOR_STMT,
        FUNC_DECL_STMT,
        RETURN_STMT
} StmtType;
struct Stmt {
        StmtType type;
        union {
                struct {
                        char *type;
                        char *var_name;
                        Expr *value;
                } variable_decl;
                struct {
                        Expr *condition;
                        StmtList *stmts;
                        Stmt *elif_stmt;
                } if_stmt;
                struct {
                        StmtList *stmts;
                } else_stmt;
                struct {
                        StmtList *stmts;
                        Expr *condition;
                } while_stmt;
                struct {
                        Expr *ret_value;
                } return_stmt;
                struct {
                        StmtList *stmts;
                        Expr *count;
                } for_stmt;
                struct {
                        char *func_name;
                        Parameter *params;
                        StmtList *stmts;
                } func_decl;
        };
};

struct StmtList {
        Stmt **statements;
        int count;
};

Expr *create_int_literal(char *value);
Expr *create_identifier_literal(char *value);
Expr *create_bool_literal(char *value);
Expr *create_string_literal(char *value);
Expr *create_binary_expr(char *operator, Expr * left, Expr *right, ExprType type);
Stmt *create_variable_decl_stmt(char *type, char *var_name, Expr *value);
Stmt *create_var_assignment_stmt(char *var_name, Expr *value);
Stmt *create_if_stmt(Expr *conditon);
Stmt *create_else_stmt();
Stmt *create_for_stmt(Expr *iterate);
Stmt *create_while_stmt(Expr *condition);
Stmt *create_func_decl_stmt(char *func_name, Parameter *params);
Expr *create_funct_call(char *func_name, Argument *args);
StmtList *create_stmt_list();
void print_stmt_list(StmtList *list);
void print_expr(Expr *expr);
void print_param(Parameter *head);
void print_args(Argument *head);
#endif // !AST_H
