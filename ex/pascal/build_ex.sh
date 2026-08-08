mgclex pascal.mlex lexer_matrix.h -c
mgcpgen -l pascal_sub.mgrm
cp -R ../common/* .
mv mgcpgen_out.hpp parse/
mv lexer_matrix.h lex/
cp actions_pasc.hpp parse/actions.hpp
g++ -g repl.cpp -o pascal
./pascal test_scripts/example.pas
