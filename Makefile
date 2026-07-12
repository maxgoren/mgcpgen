mgcpgen:
	g++ -c src/calc_firsts.cpp
	g++ -c src/calc_follows.cpp
	g++ -c src/cfg.cpp
	g++ -c src/production.cpp
	g++ -c src/mgcpgen.cpp 
	g++ *.o -o mgcpgen

install:
	cp mgcpgen /usr/local/bin

clean:
	rm *.o
	rm mgcpgen