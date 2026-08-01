# mgcpgen

`mgcpgen` is a Parser Generator designed to be used with the lexer generator `mgclex`.
It reads in a grammar specification in BNF format and outputs 
a table-driven, CLR(1) or LALR(1) parser. 

Examples can be found in the `ex/` folder.

mgcpgen has algorithms for:
```
  1)  Computing First Sets of a Grammar
  2)  Computing Follow Sets of a Grammar
  3)  Generate Parsing Tables for:
      3a) LALR(1) parser
      3b) CLR(1) parser
```

# Usage
To generate an LR parser from a BNF grammar:
```
  mgcpgen <parser type> <input file name> [output file name]
```

Create CLR parser using default output name
```
  mgcpgen -c ex/simple_expr.mgrm
```

Create LALR parser into `my_header.hpp`
```
  mgcpgen -l ex/simple_expr.mgrm my_header.hpp
```
