# mgcpgen

`mgcpgen` is a Parser Generator designed to be used with the lexer generator `mgclex`.
It reads in a grammar specification in BNF format and outputs a table-driven top-down parser.

The parser expects an accompanying actions.h file with user supplied routines to be performed upon
accepting a production rule. examples can be found in the `ex/` folder.


mgcpgen has algorithms for:
```
  1)  Computing First Sets 
  2)  Computing Follow Sets
  3)  Validating that a supplied grammar is LL(1)
  4) Computing the transition table for a top down iterative parser.
```

