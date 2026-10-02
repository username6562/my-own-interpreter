#include "../include/interpreter.h"
#include "../include/lexer.h"
#include "../include/parser.h"
#include "../include/value.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *get_file_ext(char *file_name) {
        char *ext = strchr(file_name, '.');

        if (ext == NULL) {
                fprintf(stderr, "Cannot Compile Code Invalid Extension");
                exit(1);
        }

        return ext;
}
int main(int argc, char *argv[]) {

        if (argc < 2) {
                fprintf(stderr, "Invalid Arguments Given\n");
                exit(1);
        }

        char *file_name = argv[1];

        /*
         * Example If The File is src.txt
         * The Function "get_file_ext" returns .txt And Since The '.' is not needed
         * The "+ 1" moves the Base Index Making .txt txt
         */

        char *ext = get_file_ext(file_name) + 1;

        if (strcmp(ext, "usr") != 0) {
                fprintf(stderr, "Expected Extension \"usr\" But Got %s ", ext);
                exit(1);
        }

        char *src_code = read_file(file_name);

        if (src_code == NULL) {
                printf("Failed to read file\n");
                return 1;
        }

        TokenList list = create_token_list(src_code);

        /* Initial pos starts at -1 because parse_statement starts function with
         * get_next_token which will increment pos by 1 making pos 0 at the
         * beginning of function
         */
        int pos = -1;

        StmtList *stmts = parse(list, &pos);

        init_global_scope();
        init_function_list();
        for (int i = 0; i < stmts->count; i++) {
                Stmt *stmt = stmts->statements[i];
                eval_stmts(stmt, global_scope);
        }
        // printf("number of variables are %d", global_scope->count);
        // print_scope(global_scope);
        //
        free(src_code);
        free_token_list(&list);
        return 0;
}
