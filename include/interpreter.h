#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "../include/ast.h"
#include "../include/value.h"

Variable *eval_stmts(Stmt *stmt, Scope *current_scope);
#endif // !INTERPRETER_H
