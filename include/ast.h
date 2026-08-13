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
} ExprType;

typedef struct Expr Expr;
typedef struct Stmt Stmt;
typedef struct StmtList StmtList;
struct Expr {
        char *value;
        ExprType type;
        Expr *left;
        Expr *right;
};

typedef enum {
        VAR_DECL_STMT,
        VAR_REASSIGN_STMT,
        IF_STMT
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
StmtList *create_stmt_list();
void print_stmt_list(StmtList *list);
void print_expr(Expr *expr);
#endif // !AST_H
