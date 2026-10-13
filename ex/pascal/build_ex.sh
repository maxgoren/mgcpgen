cp -R ../common/* .
mgcpgen -lx pascal_sub.mgrm
mv lexer_matrix.h lex/
mv mgcpgen_out.hpp parse/
cp actions_pasc.hpp parse/actions.hpp
g++ -g repl.cpp -o pascal

