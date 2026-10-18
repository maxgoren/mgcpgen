mgcpgen:
	g++ -c -g src/cfg/calc_nullable.cpp
	g++ -c -g src/cfg/calc_firsts.cpp
	g++ -c -g src/cfg/calc_follows.cpp
	g++ -c -g src/cfg/cfg.cpp
	g++ -c -g src/cfg/production.cpp
	g++ -c -g src/lr_gen/lr_item.cpp
	g++ -c -g src/lr_gen/lr_state.cpp
	g++ -c -g src/lr_gen/serialize.cpp
	g++ -c -g src/lr_gen/lr_gen.cpp
	g++ -c -g src/mgcpgen.cpp 
	g++ -g *.o -o mgcpgen -lmgclex

install:
	cp mgcpgen /usr/local/bin

clean:
	rm *.o
	rm mgcpgen