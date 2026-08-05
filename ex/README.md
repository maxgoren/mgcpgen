 # MGCPgen LALR(1) Examples

 ## Grammar Files

included are a number of grammar files to use as examples

```
pascal_sub.mgrm - Pascal Subset Grammar from appendix of "dragon book"
plain_expr.mgrm - simple expression grammar with +, -, *, /, unary minus and assignment
ambig_expr.mgrm - an example of using the set_token_prec keywork to for controlled ambiguity
balpar.mgrm - balanced parens grammar examples.
small_lang_ex.mgrm - an example grammar file for a simple imperative toy language
small_land_ambig.mgrm - same as above but with flattened expression grammar
```

```
    pascEx.sh - this script will generate the lexer and parser for the pascal subset, 
                set up the example folder,  build the the interpreter and execute example.pas
    smallEx.sh - does the same but for the small lang grammar file.
```

all of the above will work with include "actions.hpp".

When writing your own action routines any function to be used as an action routine should expect 
a vector of ast objects and return a single ast object representing the reduced production.