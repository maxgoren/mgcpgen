# mgcpgen

`mgcpgen` is a Parser Generator designed to be used with the lexer generator `mgclex`.
It reads in a grammar specification in BNF format and outputs a table-driven.

Examples can be found in the `ex/` folder.


mgcpgen has algorithms for:
```
  1)  Computing First Sets 
  2)  Computing Follow Sets
  3)  Generate Parsing Tables for:
      3a) the transition table for an LL(1) iterative parser.
      3b) the action/goto tables for an SLR(1) parser
```

