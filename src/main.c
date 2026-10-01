#include "../include/interpreter.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/value.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
        printf("=== Starting program ===\n");

        char *string = read_file("source.txt");
        if (string == NULL) {
                printf("Failed to read file\n");
                return 1;
        }
        TokenList list = create_token_list(string);
        print_tokens(list);

        /* Initial pos starts at -1 because parse_statement starts function with
         * get_next_token which will increment pos by 1 making pos 0 at the
         * beginning of function
         */
        int pos = -1;

        StmtList *stmts = parse(list, &pos);
        print_stmt_list(stmts);

        init_global_scope();
        init_function_list();
        for (int i = 0; i < stmts->count; i++) {
                Stmt *stmt = stmts->statements[i];
                eval_stmts(stmt, global_scope);
        }
        // printf("number of variables are %d", global_scope->count);
        // print_scope(global_scope);
        //
        free(string);
        free_token_list(&list);
        return 0;
}
