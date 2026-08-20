mgclex expr.mlex lexer_matrix.h -c
mgcpgen -l ambig_expr.mgrm
cp -R ../common/* .
mv mgcpgen_out.hpp parse/
mv lexer_matrix.h lex/
cp actions_calc.hpp parse/actions.hpp
g++ -g repl.cpp -o repl
./repl
