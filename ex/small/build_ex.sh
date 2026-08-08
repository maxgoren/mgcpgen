mgclex small_lang.mlex lexer_matrix.h -c
mgcpgen -l small_lang_ex.mgrm
cp -R ../common/* .
mv mgcpgen_out.hpp parse/
mv lexer_matrix.h lex/
cp actions_small_lang.hpp parse/actions.hpp
g++ -g repl.cpp -o repl
./repl test_scripts/example.owl
