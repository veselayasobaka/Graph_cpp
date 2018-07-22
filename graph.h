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
    string child_range = "";
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
                    + to_string(colour) + " " + child_range + "           ";
            for(auto it = bounds.begin(); it != bounds.end(); ++it)
            {
                a += "(" + to_string(it->first) + ", " + to_string(it->second) + ")";
            }
            return a;
        }
    friend ostream &operator<<(ostream &os, const Vertex& b)
    {
        return os << string(b);
    }
    friend bool operator==(const Vertex &vl, const Vertex &vr)
    {
        return vl.number == vr.number;
    }
};

//prototypes of functions to sort the Graph
bool sortlevels(Vertex &v1, Vertex &v2);
bool waytosort(Vertex &v1, Vertex &v2);


// Graph - class for interaction with user.
//Graph keeps vector of vertexes and translates adjacency matrix to adjacency list(vector of vertexes).
//it's metods also cononize graph.

class Graph
{
private:
    map<int, Vertex> vertexes;     //graph keeps data in map - for fast acces by key
public:
    Graph(){}
    Graph(const Ref<const MatrixXf>& m1);
    Graph(int q)
    {
        for (int i = 0; i < q; ++i)
        {
            Vertex v(i);
            vertexes.insert({i, v});
        }
    }
    void set_bound(int a, int b, int c)      // you don't need to use this
    {
        auto it = vertexes.find(a);
        it->second.add_bound(b, c);
        it = vertexes.find(b);
        it->second.add_bound(a, c);

    }

    string get_canon_code();        // it's our heart
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
    friend ostream &operator<<(ostream &os, const Graph& b)
        {
            return os << string(b);
        }
    void add_vertex(int a, int b)
        {
            Vertex v(a, b);
            vertexes.insert({a, v});
        }
    bool is_cycle();

    void find_cycle();


};


#endif // GRAPH_H
