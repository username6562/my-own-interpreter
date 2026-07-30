#ifndef AST_H
#define AST_H

#include <stdbool.h>

typedef enum {
        INT_LITERAL,
        IDENTIFIER_LITERAL,
        ADDITION_OP,
        SUBTRACTION_OP,
        MULTIPLICATION_OP,
        DIVISION_OP,
        ASSIGNMENT_OP
} ExprType;

typedef struct Expr Expr;

struct Expr {
        char *value;
        ExprType type;
        Expr *left;
        Expr *right;
};

typedef enum {
        VAR_DECL_STMT
} StmtType;

typedef struct {
        StmtType type;
        union {
                struct {
                        char *type;
                        char *var_name;
                        Expr *value;
                        bool is_declared;
                } variable_decl;
        };

} Stmt;

typedef struct {
        Stmt **statements;
        int count;
} StmtList;

Expr *create_int_literal(char *value);
Expr *create_identifier_literal(char *value);
Expr *create_binary_expr(char *operator, Expr * left, Expr *right,
                         ExprType type);
Stmt *create_variable_decl_stmt(char *type, char *var_name, Expr *value,
                                bool is_declared);
StmtList *create_stmt_list();
void print_stmt_list(StmtList *list);
void print_expr(Expr *expr);
#endif // !AST_H
