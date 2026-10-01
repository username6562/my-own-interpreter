#ifndef PARSER_H
#define PARSER_H

#include "../include/ast.h"
#include "../include/lexer.h"

Stmt *parse_statement(TokenList list, int *pos, bool is_in_function);
StmtList *parse(TokenList list, int *pos);
#endif // !PARSER_H
