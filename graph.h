#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
#include <eigen3/Eigen/Dense>
#include <queue>
using namespace std;
using namespace Eigen;

//Bound - keep two numbers - number of vertex, bounded with ours
//and the weigth of this bound
struct Bound
{
    int number_vertex;
    int weigth;
    Bound(int n, int w): number_vertex(n), weigth(w){}
    operator string() const
        {
            return string("(") + to_string(number_vertex) + ", " + to_string(weigth) + ")";
        }
    ~Bound(){}
    friend std::ostream &operator<<(std::ostream &os, const Bound& b)
    {
        return os << string(b);
    }
    friend bool operator ==(const Bound &lhs, const Bound &rhs);
};
//vertex - structure to keep all data about some vertex:
//it's number, level, colour(mass), range of it's children and vector of bounds
struct Vertex
{
    int number = 0;
    int level = 0;
    int colour = 0;
    int child_range = 0;
    vector<Bound> bounds;
    Vertex(int n): number(n){}
    Vertex(int n, int colour): number(n), colour(colour){}
    void add_bound(const Bound b)
    {
        bounds.push_back(b);
    }
    operator string() const
        {
            string a = ("") + to_string(number) + " " + to_string(level) + " "
                    + to_string(colour) + " " + to_string(child_range) + " ";
            for(const auto& bound: bounds)
            {
                a+=bound;
            }
            return a;
        }
    friend std::ostream &operator<<(std::ostream &os, const Vertex& b)
    {
        return os << string(b);
    }
    void move_bound_to_end(const int &b, const int &c)
    {
        Bound temp(b, c);
        auto it = find(bounds.begin(), bounds.end(), temp);
        if (it!=bounds.end())
            {
            rotate(it, it+1, bounds.end());
            }
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
        for (auto it = vertexes.begin();  it != vertexes.end(); ++it)
        {
            cout << *it;
            cout << endl;
        }
    }
    void set_colour(int n, int c)
    {
        vertexes[n].colour = c;
    }
    void set_colour()
    {
        for (auto it = vertexes.begin();  it != vertexes.end(); ++it)
        {
            cout << "Vertex " << it->number << ':';
            int a = 0;
            cin >> a;
            vertexes[it->number].colour = a;
        }
    }
    void set_bound(int n)
    {
        cout << "Vertex " << n << ':';
        int i = 0;
        int j = 0;
        cin >> i >> j;
        Bound b(i, j);
        vertexes[n].add_bound(b);
    }
    void set_bound(int a, int b, int c)
    {
        Bound b1(a, c);
        Bound b2(b, c);
        vertexes[a].add_bound(b2);
        vertexes[b].add_bound(b1);
    }

    void set_bound()
    {
        for (auto it = vertexes.begin();  it != vertexes.end(); ++it)
        {
            cout << "Vertex " << it->number << ':';
            int i = 0;
            int j = 0;
            cin >> i >> j;
            Bound b(i, j);
            vertexes[it->number].add_bound(b);
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
