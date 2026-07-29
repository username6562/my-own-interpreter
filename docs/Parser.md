the parser is the second step in making a fully functional interpreter it turns a list of tokens into a Tree Node

# THE `parse_expr` FUNCTION

**The parse_expr function shot for parse_expression inputs a stream of tokens provided by the lexer and provides operator precedence through the help of an algorithm called `Pratt Parsing`
`Pratt Parsing` uses the concept of binding power  to provide operator precedence
Where the higher the binding power the lower the operator goes down in the tree

| Operator | Binding Power |
| -------- | ------------- |
| `+`      | 10            |
| `-`      | 10            |
| `*`      | 20            |
| `/`      | 20            |

Lets use an example expression `1 + 2 * 3 + 4`
```txt
Index -   0   1   2   3   4   5  6
Token -  '1' '+' '2' '*' '3' '+' 4' 
```

# ORIGINAL FUNCTION
 **Because in the `../src/main.c` pos starts at -1
 When we call `parse_primary` in order to get the next the Token `1` in our `parse_primary` we use `get_next_token` to increment pos by 1 now making pos 0** with the current binding power of 0
 - Left = 1 
 - In the loop we call get  we save the current position of pos in a variable called `save_pos`
  The `saved_pos` variable is used to save the position before peeking at the next token. If we decide NOT to process the operator (because it has lower binding power), we restore the position so the caller can process it instead.
 - We then use `get_next_token` to peek into the token stream  to get the `+` Token. Making pos 1 now
 -  We use `get_binding_power('+')` to return the next_binding power which is 10
 - Because 0 is not greater that 10 the loop doesn't break 
 - Because the loop doesn't break the function generates a `ADDITION_NODE` and the left of the addition node becomes `1`
 - To get the right node the function recursively calls itself making a new copy of itself with the new current_binding_power becoming 10

## COPY 1 `parse_expr(list , pos , 10)`
**Copy 1 starts the same by using `parse_primary` function to get the left value `2` making pos to become 2
- `saved_pos` now becomes 2
- It then uses `get_next_token` to return the operator after it `*` making pos become 3
- Then it gets the next_bp with the function `get_binding_power('*')` returning 20
- Because 10 is not greater than 20 the loop doesn't break and creates a `MULTIPLICATION_NODE` 
- The function then assigns left 2 to be its left hand side and recursively calls itself again with 20 now becoming the new `current_binding_power`
## COPY 2 `parse_expr(list , pos , 20)`
**Copy 2 gets the left value -> 3 now pos becomes 4
- `saved_pos` then becomes 4
- We use `get_next_token` to get the next operator `+` pos now becomes 5
- WE get the next binding power with `get_binding_power('+')` -> 10
- Because 20 is greater than 10
- We then assign pos to be `saved_pos` so pos is now 4 again
- The loop breaks and returns the left value 3
- 3 then becomes the right value of `COPY 1` making `COPY 1`'s opNode look like this
```txt
                             *
                        /         \
                        2          3
```

**And because `COPY 1` is the right node of the original function**
The full AST looks like this

```txt
                      +       
             /                \
            1               *
                        /         \
                        2          3
```
Now that the Tree Node has been gotten we make left the root node cause we are parsing from left to right

BUT BECAUSE THE LOOP OF THE ORIGINAL FUNCTION HASNT BEEN BROKEN WE CONTINUE
- We get the next operator because we assigned pos to be 4 previously we can now peek to the correct operator
- Without that reassignment the next token would have been pointing to 4 and not the `+` operator
- After getting the `+` operator we get its binding power -> 10
- Since 0 is not greater than 10
- the loop continues creates the `ADDITION_NODE` and assigns it left to be the Tree Node Above
- And it gets the right node by recursion

**NOTE: the loop terminates when a non operator token is found or when the current binding power is greater than the next binding power**

### FINAL AST REPRESENTATION
```txt
                            +       
                     /            \
                                      4
                     +       
             /                \
            1               *
                        /         \
                        2          3
```

