the parser is the second step in making a fully functional interpreter it turns a list of tokens into Statements and Expression

# THE `parse_expr` FUNCTION

**The parse_expr function shot for parse_expression inputs a stream of tokens provided by the lexer and provides operator precedence through the help of an algorithm called `Pratt Parsing`
`Pratt Parsing` uses the concept of binding power  to provide operator precedence
Where the higher the binding power the lower the operator goes down in the tree

## WHY PRATT PARSING
- **Pratt Parsing was chosen because of its simplicity of getting precedence with binding power (a simple numeric value) . It replaces a lot of chained grammar rules that are complex to follow up**
- **Scalability: It is easy to add new operators , prefixes , etc**


| Operator   | Binding Power |
| ---------- | ------------- |
| `+`        | 15            |
| `-`        | 15            |
| `*`        | 20            |
| `/`        | 20            |
| `<` / `<=` | 10            |
| `>`/ `>=`  | 10            |
| `==`       | 10            |
| `=`        | 5             |
## pseudo-code
```text
def parse_expr(current_bp):
	left = parse_primary();
	
	while true:
		if (current_bp >= next_bp) 
			break
		op = parse_expr(next_bp)
		left = op
	return left
```

**Functions gets terminated either when the `current_bp` is greater than the `next_bp` or it doesn't find an operator after the number**



Lets use an example expression `1 + 2 * 3 + 4`
```txt
Index/Pos -   0   1   2   3   4   5  6
Token -      '1' '+' '2' '*' '3' '+' '4' 
```

# ORIGINAL FUNCTION 
  - **Function Call:** `parse_expr(list , pos , current_bp =0)`
  - **Initial** `pos` = `-1`
## Processing Left Value
- The function starts by calling `parse_primary` to consume token and create an `INT_LITERAL(1)`
-  `left` = `1`
- `pos` now becomes `0`

## Main Loop Begins

**1. Saves `pos` value:** `int saved_pos = pos;` stores 0 in case the loop breaks
**2. Consumes Token:**  Calls `get_next_token` to get `+` . `pos` now become 1
**3. Checks Precedence:** `next_bp = get_binding_power('+')` which returns 15
**4. Evaluates Condition:**
 - Condition `if (current_bp >= next_bp) break;`
- Loop continues because condition is false
**5 Build Node:**
- Creates an `ADDITION_NODE`
- Calls `create_binary_expr()` to create the tree
- Assigns its left to be `INT_LITERAL 1` `expr->left = left;`
**6. Recursion for Right Node:**
- Creates a new copy of function to resolve the remaining part of the expression
## COPY 1
**CURRENT FUNCTION STATE**
- Calls `parse_primary` to consume current token and create `INT_LITERAL` `2`
- `pos` = `2`
- `current_bp`  = 10
## COPY 1 Loop Begins
**1. Saves `pos` value:** `int saved_pos = pos;` stores 2 in case the loop breaks
**2. Consumes Token:**  Calls `get_next_token` to get `*`. `pos` now become 3
**3. Checks Precedence:** `next_bp = get_binding_power('*')` which returns 20
**4. Evaluates Condition:**
 - Condition `if (current_bp >= next_bp) break;`
- Loop continues because condition is false
**5 Build Node:**
- Creates an `MULTIPLICATION_NODE`
- Calls `create_binary_expr()` to create the tree
- Assigns its left to be `INT_LITERAL 2` `expr->left = left;`
**6. Recursion for Right Node:**
- Creates a new copy of function to resolve the remaining part of the expression

## COPY 2
**CURRENT FUNCTION STATE**
- Calls `parse_primary` to consume current token and create `INT_LITERAL` `3`
- `pos` = `4`
- `current_bp`  = 20
## COPY 2 Loop Begins
**1. Saves `pos` value:** `int saved_pos = pos;` stores 4 in case the loop breaks
**2. Consumes Token:**  Calls `get_next_token` to get `+`. `pos` now become 5
**3. Checks Precedence:** `next_bp = get_binding_power('+')` which returns 15
**4. Evaluates Condition:**
 - Condition `if (current_bp >= next_bp) break;`
- Loop breaks because condition is true
- pos now becomes `save_pos`
5 **Skips Creating Expr Node** it returns the `left (3)`  
***Note: Because COPY 2 was called to get the right node of the COPY 1 the left node (3) gotten will be the right node of COPY 1  Making COPY 1 look like this***

```txt
    *
   / \
  2   3
```
***And since COPY 1 is the right Node of the original function the full Expr Node will look like this***
```txt
      +
     / \
    1   *
       / \
      2   3

```

## ORIGINAL FUNCTION LOOP CONTINUES
## Current State of function
-  `left` = **the expr node above**
- `pos` = `4`
- `current_bp` = 0
**1 Saves `pos` value:** `int saved_pos = pos;` stores 4 in case the loop breaks
**2. Consumes Token:**  Calls `get_next_token` to get `+`. `pos` now become 5
**3. Checks Precedence:** `next_bp = get_binding_power('+')` which returns 15
**4. Evaluates Condition:**
 - Condition `if (current_bp >= next_bp) break;`
 - Loop continues because its true
**5 Build Node:**
- Creates an `ADDITION_NODE`
- Calls `create_binary_expr()` to create the tree
- Assigns its left to be **tree node above** `expr->left = left;`
**6. Recursion for Right Node:**
- Creates a new copy of function to resolve the remaining part of the expression
- The copy then returns its right node to be 4
### FINAL TREE NODE
```txt
          + 
         / \
        +   4
       / \
      1   *
         / \
        2   3

```



