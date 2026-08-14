The Lexer has three main jobs to do in the interpreter:
- To read the current file your working on and turn it into a string
- Reads the string 
- Generates a list of tokens for the parser to user

## Useful Data Types in The Lexer
| Type        | Usage                                                                                  |
| ----------- | -------------------------------------------------------------------------------------- |
| `Token`     | Represents the smallest meaningful categories of code like keywords , variables , etc. |
| `TokenType` | Stores all possible types supported by the Lexer in an enum                            |
| `TokenList` | Stores an array of tokens                                                              |

# Pipeline of the Lexer
There are three main functions needed for the pipeline

| Function            | Use                                                                                                                     |
| ------------------- | ----------------------------------------------------------------------------------------------------------------------- |
| `read_file`         | inputs the name of the file and returns all the contents in a string                                                    |
| `tokenize`          | inputs a string and returns a token with the correct type                                                               |
| `create_token_list` | loops through the  source code created by `read_file` ,  divides it into chunks for `tokenize`  to create the functions |
The Lexer has three distinct phases
### Stage 1: Reading the Source File
The `read_file(char *file_name)` is responsible for this stage .
- It opens the file using the `file_name` parameter and transforms the code into a string
### Stage 2: Transforming The Code
This stage is handled by `create_token_list(char *source)` .
- It loops through the source code created by `read_file(char *source)` 
- Divides it into categorized chunks and hands it over to `tokenize` function to handle create the token and assign to its correct type.
- The tokens created by `tokenize` is added to the token array in the `TokenList` created in `create_token_list(const char *source)`