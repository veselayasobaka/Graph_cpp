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
Graph readFile(const string &s)
{
    ifstream file(s);
    string buff, numb;
    int a = 0;
    int b = 0;
    int c = 0;
    Graph g1;
    string ch = "#";
    while(getline(file, buff))
    {
        if (buff==ch)
            break;
        stringstream ss(buff);
        ss >> a >> b;
        g1.addNode(a, b);

    }
    while(getline(file, buff))
    {
        stringstream ss(buff);
        ss >> a >> b >> c;
        g1.setEdge(a, b, c);
    }
    return g1;
}
// for tests - give it matrix of adjacency, it'll write it to file
void writeMatrixToFile(const Ref<const MatrixXi>& m1)
{
    cout << "Write matrix to filename>";
    string s;
    cin >> s;
    ofstream file(s);
    if (m1 == m1.transpose())
    {
        int size = m1.rows();
        for (int i = 0; i < size; ++i)
        {
            file << i << ' ' << endl;
        }
        file << '#' << endl;
        for (int i = 0; i < size; ++i)
        {
            for(int j = i; j < size; ++j)
            {
                if (m1(i, j)!=0)
                file << i << ' ' << j << ' ' << m1(i, j) << endl;
            }
        }
        file.close();
    }
    else
        cout << "Wrong matrix! Matrix should be square!" << endl;
}
