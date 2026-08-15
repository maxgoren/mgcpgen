 # MGCPgen LALR(1) Examples

 ## Examples

included are a number of folders each containing example grammars.

```
calc - example expression grammars showing how to organize operator 
       precedence either at the grammar level or through precedence 
       declarations on a flat grammar
pascal - Pascal Subset Grammar from appendix of "dragon book"
tiger - the Tiger language from Appels Modern Compiler series.
```

in additionm, each folder contains two scripts, build_ex.sh and cleanup.sh to help build the examples 
```
    build_ex.sh - this script will generate the lexer and parser for the choosen example, 
                set up the example folder,  build the the interpreter and execute a test script
    cleanup.sh - well, its not doing the dishes.
```

When writing your own action routines any function to be used as an action routine should expect 
a vector of ast objects and return a single ast object representing the reduced production.
