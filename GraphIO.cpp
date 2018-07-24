#include <iostream>
#include <sstream>
#include <fstream>
#include <eigen3/Eigen/Dense>
#include "graph.h"
using namespace std;
using namespace Eigen;

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
    string buff;
    int node1 = 0;
    int node2 = 0;
    int edge = 0;
    Graph g1;
    string ch = "#";
    while(getline(file, buff))
    {
        if (buff==ch)
            break;
        stringstream ss(buff);
        ss >> node1 >> node2;
        g1.addNode(node1, node2);

    }
    while(getline(file, buff))
    {
        stringstream ss(buff);
        ss >> node1 >> node2 >> edge;
        g1.setEdge(node1, node2, edge);
    }
    return g1;
}
// for tests - give it matrix of adjacency, it'll write it to file
void writeMatrix(const Ref<const MatrixXi> &matrix)
{
    cout << "Write matrix to filename>";
    string filename;
    cin >> filename;
    ofstream file(filename);
    if (matrix == matrix.transpose())
    {
        int size = matrix.rows();
        for (int i = 0; i < size; ++i)
        {
            file << i << ' ' << endl;
        }
        file << '#' << endl;
        for (int i = 0; i < size; ++i)
        {
            for(int j = i; j < size; ++j)
            {
                if (matrix(i, j)!=0)
                file << i << ' ' << j << ' ' << matrix(i, j) << endl;
            }
        }
        file.close();
    }
    else
        throw logic_error("MATRIX NOT SIMMYTRIC!");
}
void writeCode(const string &filename, const string &code)
{
    ofstream file(filename);
    file << code;
    file.close();
}
