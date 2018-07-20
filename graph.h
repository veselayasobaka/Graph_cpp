#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <eigen3/Eigen/Dense>
#include <map>
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
    vector<Vertex> vertexes;
public:
    Graph(){}
    Graph(MatrixXf &m1);
    Graph(int q)
    {
        for (int i = 0; i < q; ++i)
        {
            Vertex v(i);
            vertexes.push_back(v);
        }
    }
    void show_graph() const
    {
        for(const auto& vertex: vertexes)
        {
            cout << vertex;
            cout << endl;
        }
    }
    void set_colour(int n, int c)
    {
        vertexes[n].colour = c;
    }
    void set_colour()
    {
        for(const auto& vertex: vertexes)
        {
            cout << "Vertex " << vertex.number << ':';
            int a = 0;
            cin >> a;
            vertexes[vertex.number].colour = a;
        }
    }

    void set_bound(int a, int b, int c)
    {
        vertexes[a].add_bound(b, c);
        vertexes[b].add_bound(a, c);
    }

    void set_bound()
    {
        for(const auto& vertex: vertexes)
        {
            cout << "Vertex " << vertex.number << ':';
            int i = 0;
            int j = 0;
            cin >> i >> j;
            vertexes[vertex.number].add_bound(i, j);
        }
    }
    void set_levels();
    operator string() const
        {
            string a;
            for(const auto& vertex: vertexes)
            {
                a+=vertex;
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
        vertexes.push_back(Vertex(a, b));
    }

};


#endif // GRAPH_H
