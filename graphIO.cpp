#include <iostream>
#include <sstream>
#include <fstream>
#include <cerrno>
#include "graph.h"
using namespace std;

//function to read from file
// matrix should be in format like this:
//0 2
//1 3
//#
//0 1 12
// 0, 1 - numbers of nodes, 2,3 - their mass factor (colour in my terminology), 12 - weigth of edge between them
Graph readFile(const string &filename)
{
    ifstream file(filename);
    if (file.is_open())
    {
            string buff;
            int node1 = 0;
            int node2 = 0;
            int edge = 0;
            Graph g1;
            while(getline(file, buff) && buff != "#")
            {
                stringstream ss(buff);
                if((ss >> node1).fail())
                {
                    cerr << "Invalid file format!" << endl;
                    exit(EXIT_FAILURE);
                }
                if((ss >> node2).fail())
                {
                    node2 = 0;
                }
                g1.addNode(node1, node2);

            }
            while(getline(file, buff))
            {
                stringstream ss(buff);
                if((ss >> node1).fail())
                {
                    cerr << "Invalid file format!" << endl;
                    exit(EXIT_FAILURE);
                }
                if((ss >> node2).fail())
                {
                    cerr << "Invalid file format!" << endl;
                    exit(EXIT_FAILURE);
                }
                if((ss >> edge).fail())
                {
                    cerr << "Invalid file format!" << endl;
                    exit(EXIT_FAILURE);
                }
                g1.setEdge(node1, node2, edge);
            }
            return g1;
    }
    else
    {
        cerr << "File not found!" << endl;
        exit(EXIT_FAILURE);
    }
}

void writeCode(const string &filename, const string &code)
{
    ofstream file(filename);
    file << code;
    file.close();
}
