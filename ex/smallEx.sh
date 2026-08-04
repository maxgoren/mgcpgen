mgclex grammars/lex_specs/small_lang.mlex lex/lexer_matrix.h -c
mgcpgen -l grammars/small_lang_ex.mgrm
mv mgcpgen_out.hpp parse/
cp parse/actions_small_lang.hpp parse/actions.hpp
g++ -g repl.cpp -o repl
./repl example.owl
