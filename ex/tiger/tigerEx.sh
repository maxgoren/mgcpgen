mgclex grammars/lex_specs/tiger.mlex lex/lexer_matrix.h -c
mgcpgen -l grammars/tiger.mgrm
mv mgcpgen_out.hpp parse/
cp parse/actions_tiger.hpp parse/actions.hpp
g++ -g repl.cpp -o repl
./repl example.tig