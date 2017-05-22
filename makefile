
FLAGS = -g  -O3 -Wl,--no-as-needed -ldl -Wall -pedantic -Wextra -std=c++11

all: clean bsackst1_test

bsackst1_test: bsackst1_test.o 
	g++ $(FLAGS)  bsackst1_test.o  -o bsackst1_test

bsackst1_test.o: bsackst1_test.cpp Map.hpp 
	g++ $(FLAGS)	-c bsackst1_test.cpp -o bsackst1_test.o

clean:
	rm -f *.o bsackst1_test
