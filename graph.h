#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
#include <queue>
#include <eigen3/Eigen/Dense>

using namespace std;
using namespace Eigen;

//node - structure to keep all data about some node:
//it's number, level, colour(mass), range of it's children and vector of edges
struct Node
{
    int number = 0;
    int level = 0;
    int colour = 0;
    string child_range = "";
    map<int, int> edges;
    Node(int n): number(n){}
    Node(int n, int colour): number(n), colour(colour){}
    void addEdge(int a, int b);
    operator string() const;
    friend ostream &operator<<(ostream &os, const Node& b);
    friend bool operator==(const Node &vl, const Node &vr);
};

//prototypes of functions to sort the Graph
bool sortlevels(Node &v1, Node &v2);
bool waytosort(Node &v1, Node &v2);


// Graph - class for interaction with user.
//Graph keeps vector of _nodes and translates adjacency matrix to adjacency list(vector of _nodes).
//it's metods also cononize graph.

class Graph
{
private:
    map<int, Node> _nodes;     //graph keeps data in map - for fast acces by key
public:
    Graph(){}
    Graph(const Ref<const MatrixXi>& m1);
    Graph(int q);
    void setEdge(int a, int b, int c);
    operator string() const;
    friend ostream &operator<<(ostream &os, const Graph& b);
    void addNode(int a, int b);
    bool isCycle();
    string getCanonCode();        // it's our heart
};

#endif // GRAPH_H
