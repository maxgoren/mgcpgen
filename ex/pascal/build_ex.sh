#mgclex pascal.mlex lexer_matrix.h -c
#mv lexer_matrix.h lex/
cp -R ../common/* .
mgcpgen -l pascal_sub.mgrm
mv mgcpgen_out.hpp parse/
cp actions_pasc.hpp parse/actions.hpp
g++ -g repl.cpp -o pascal
./pascal test_scripts/gcd.pas