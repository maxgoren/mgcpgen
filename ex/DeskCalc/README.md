# Desk Calculator

This is an example of using mgcpgen to implement a desk calculator.

# Generating the lexer

The lexical specification for tokens is laid out in the file DeskCalc.mlex

To generate the resultant DFA, you can run:

```
    mgclex DeskCalc.mlex include/lexer_matrix.h
```

A pre-generated version already exists in the include folder.


# Generating the parser
the grammar for our calculator is in DeskCalc.mgrm, as was the case for the lexer
a pre-generated version of our parser already exists in the include folder, but if you want
to add productions and re-generate it you can use

```
    mgcpgen DeskCalc.mgrm include/exprs.hpp
```


# Compiling the result

To build the example, simply compile DeskCalc.cpp with your systems C++ compiler:

```
    g++ DeskCalc.cpp -o DeskCalc
```