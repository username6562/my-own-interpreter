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

The function starts off by getting the first number `1` producing an AST Number Node
Then it gets the next token and uses the `get_binding_power` function to return the `next_bp (next_binding_power)` power of `+` which is `10`. It makes the check
```c
  if (current_binding_power >= next_bp) {
    break;
}
```
becuase 0 is not greater than 1 the loop continues and creates an Addition Node and assigns its left node to be number node 
So now the current ast is looking like this
```txt
                            +
                        /        \
                        1
```

To find its right node it recursively calls itself so a new copy is made with 10 now becoming the `current_binding_power`

## COPY 1
The function starts  again by getting the next number which is  `2` and creates an AST Number Node. It then peeks ahead gets the next token which is `*` . It has the binding power of 20 and checks  makes this check again

```c
  if (current_binding_power >= next_bp) {
    break;
}
```
Because 10 is not greater than 20 the loop continues and makes the `MULTIPLICATION_NODE` and assigns 2 to be its left hand side . It then recursively makes another copy in order to find `copy 1`'s right hand side making 20 the `current_binding_power` of `copy 2`

Now the ASTNode  is currently looking like this because `COPY 1` is the right hand side of the original function
```txt
                             +
		                /         \
		                1          *
		                        /
		                        2
```

## COPY 2
`
`COPY 2` starts the same by getting the next number which is `34 and creates a Number Node. it then gets the next  token which is `+` having the binding power of  10. It then runs the same check

```c
  if (current_binding_power >= next_bp) {
    break;
}
```
now 20 (the current binding power of copy 2 ) is  greater than 10 the loop breaks and returns 3 to be the right hand side of `COPY 1` 
now the ASTNode of the original function looks like this
```txt
                             +
		                /         \
		                1          *
		                        /     \
		                        2      3
```
It then assigns this current node to the original left variable of the original function
Now that all the copies have been completed
The original function's while loop continues cause it hasn't been broken yet the loop continues by getting the next token which is `+` , which has a binding power of 10 .
Because the binding power of the original loop is 0 it checks 
```c
  if (current_binding_power >= next_bp) {
    break;
}
```
because 0 isn't greater than 10 the loop makes an `ADDITION_NODE` and assigns it left hand side to be the tree above and recursively called itself to get `3` as its right hand side
so now the completed ASTNode becomes this

```txt
                                     +
                             /             \
                             +               4
		                /         \
		                1          *
		                        /     \
		                        2      3
```