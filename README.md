# mgcpgen

`mgcpgen` is a Parser Generator designed to be used with the lexer generator `mgclex`.
It reads in a grammar specification in BNF format and outputs 
a table-driven, LALR(1) parser. 

Examples can be found in the `ex/` folder.

mgcpgen has algorithms for:
```
  1)  Computing First Sets of a Grammar
  2)  Computing Follow Sets of a Grammar
  3)  Generate Parsing Tables for:
      3a) the action/goto tables for an LALR(1) parser
```

# Usage
To generate an LR parser from a BNF grammar:
```
  mgcpgen grammar_file.mgrm
```
