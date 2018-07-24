.PHONY: all clean
	
all: grprj
	
clean:
			rm -rf grprj *.o
main.o: main.cpp
	g++  -std=c++11 -c -o  main.o main.cpp
graph.o: graph.cpp
	g++  -std=c++11 -c -o  graph.o graph.cpp
GraphIO.o: GraphIO.cpp
	g++  -std=c++11 -c -o  GraphIO.o GraphIO.cpp
grprj: main.o graph.o GraphIO.o
	g++  -std=c++11 -o grprj main.o graph.o GraphIO.o
