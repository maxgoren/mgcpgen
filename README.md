# mgcpgen

`mgcpgen` is a Parser Generator designed to be used with the lexer generator `mgclex`.
It reads in a grammar specification, either LL(1) or SLR(1) in BNF format and outputs 
a table-driven parser.

Examples can be found in the `ex/` folder.

mgcpgen has algorithms for:
```
  1)  Computing First Sets of a Grammar
  2)  Computing Follow Sets of a Grammar
  3)  Generate Parsing Tables for:
      3a) the transition table for an LL(1) iterative parser.
      3b) the action/goto tables for an SLR(1) parser
```
