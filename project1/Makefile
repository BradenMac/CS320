all: 
	g++ -g -c program1.cpp -o predictors.o -std=c++14
	g++ predictors.o -o predictors -std=c++14

run: all
	./predictors

checkmem: all
	valgrind ./predictors

clean:
	rm *.o predictors
