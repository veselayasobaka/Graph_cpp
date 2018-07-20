#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <eigen3/Eigen/Dense>
#include <map>
#include <queue>
using namespace std;
using namespace Eigen;

//vertex - structure to keep all data about some vertex:
//it's number, level, colour(mass), range of it's children and vector of bounds
struct Vertex
{
    int number = 0;
    int level = 0;
    int colour = 0;
    int child_range = 0;
    map<int, int> bounds;
    Vertex(int n): number(n){}
    Vertex(int n, int colour): number(n), colour(colour){}
    void add_bound(int a, int b)
    {
        bounds.insert({a,b});
    }
    operator string() const
        {
            string a = ("") + to_string(number) + " " + to_string(level) + " "
                    + to_string(colour) + " " + to_string(child_range) + " ";
            for(auto it = bounds.begin(); it != bounds.end(); ++it)
            {
                a += "(" + to_string(it->first) + ", " + to_string(it->second) + ")";
            }
            return a;
        }
    friend std::ostream &operator<<(std::ostream &os, const Vertex& b)
    {
        return os << string(b);
    }

};
//function to sort the Graph
bool sortlevels(Vertex &v1, Vertex &v2);
bool waytosort(Vertex &v1, Vertex &v2);


// Graph - class for interaction with user.
//Graph keeps vector of vertexes and translates adjacency matrix to adjacency list(vector of vertexes).
//it's metods also cononize graph.

class Graph
{
private:
    map<int, Vertex> vertexes;
public:
    Graph(){}
    Graph(MatrixXf &m1);
    Graph(int q)
    {
        for (int i = 0; i < q; ++i)
        {
            Vertex v(i);
            vertexes.insert({i, v});
        }
    }
    void set_bound(int a, int b, int c)
    {
        auto it = vertexes.find(a);
        it->second.add_bound(b, c);
        it = vertexes.find(b);
        it->second.add_bound(a, c);

    }

    void set_levels();
    operator string() const
        {
            string a;
            for(auto it = vertexes.begin(); it != vertexes.end(); ++it)
            {

                a+=it->second;
                a+="\n";
            }
            return a;
        }
    friend std::ostream &operator<<(std::ostream &os, const Graph& b)
        {
            return os << string(b);
        }
    void add_vertex(int a, int b)
        {
            Vertex v(a, b);
            vertexes.insert({a, v});
        }

};


#endif // GRAPH_H
