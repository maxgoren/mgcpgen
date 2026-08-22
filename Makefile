mgcpgen:
	g++ -c src/cfg/derives_lambda.cpp
	g++ -c src/cfg/calc_firsts.cpp
	g++ -c src/cfg/calc_follows.cpp
	g++ -c src/cfg/cfg.cpp
	g++ -c src/cfg/production.cpp
	g++ -c src/lr_gen/lr_item.cpp
	g++ -c src/lr_gen/lr_state.cpp
	g++ -c src/lr_gen/lr_gen.cpp
	g++ -c src/mgcpgen.cpp 
	g++ *.o -o mgcpgen

install:
	cp mgcpgen /usr/local/bin

clean:
	rm *.o
	rm mgcpgen