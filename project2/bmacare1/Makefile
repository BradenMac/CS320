all: 
	g++ -g -c program2.cpp -o cache-sim.o -std=c++14
	g++ cache-sim.o -o cache-sim -std=c++14

run: all
	./cache-sim

checkmem: all
	valgrind ./cache-sim

clean:
	rm *.o cache-sim
