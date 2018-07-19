.PHONY: all clean
	
all: grprj
	
clean:
			rm -rf grprj *.o
main.o: main.cpp
	g++  -std=c++11 -O0 -g -c -o  main.o main.cpp
graph.o: graph.cpp
	g++  -std=c++11 -O0 -g -c -o  graph.o graph.cpp
grprj: main.o graph.o
	g++  -std=c++11 -o grprj main.o graph.o
