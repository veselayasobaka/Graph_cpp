.PHONY: all clean
	
all: grprj
	
clean:
			rm -rf grprj *.o
main.o: main.cpp
	g++  -std=c++11 -c -o  main.o main.cpp
graph.o: graph.cpp
	g++  -std=c++11 -c -o  graph.o graph.cpp
read_file.o: read_file.cpp
	g++  -std=c++11 -c -o  read_file.o read_file.cpp
grprj: main.o graph.o read_file.o
	g++  -std=c++11 -o grprj main.o graph.o read_file.o
