mgclex grammars/lex_specs/pascal.mlex lex/lexer_matrix.h -c
mgcpgen -l grammars/pascal_sub.mgrm
mv mgcpgen_out.hpp parse/
cp parse/actions_pasc.hpp parse/actions.hpp
g++ -g repl.cpp -o repl
./repl example.pas
