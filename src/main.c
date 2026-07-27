#include "../include/lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

int main() {
    printf("=== Starting program ===\n");

    char *string = read_file("source.txt");
    TokenList list = create_token_list(string);

    if (string == NULL) {
        printf("Failed to read file\n");
        return 1;
    }

    print_tokens(list);
    free(string);
    free_token_list(&list);

    return 0;
}
