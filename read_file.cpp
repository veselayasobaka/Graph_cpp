#include <iostream>
#include <sstream>
#include <fstream>
#include "graph.h"
using namespace std;

Graph read_file(const string &s)
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
        g1.add_vertex(a, b);

    }
    while(getline(file, buff))
    {
        stringstream ss(buff);
        if (buff.empty())
            break;
        ss >> a >> b >> c;
        g1.set_bound(a, b, c);
    }
    return g1;
}

