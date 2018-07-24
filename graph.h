#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <map>
#include <eigen3/Eigen/Dense>

using namespace std;
using namespace Eigen;

//node - structure to keep all data about some node:
//it's number, level, colour(mass), range of it's children and vector of edges
struct Node
{
    Node(int number): _number(number){}
    Node(int number, int colour): _number(number), _colour(colour){}
    int _number = 0;
    int _level = 0;
    int _colour = 0;
    string _child_range = "";
    map<int, int> _edges;
    void addEdge(int node_number, int edge_weigth);
    operator string() const;
    friend ostream &operator<<(ostream &os, const Node &b);
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
    map<int, Node> _nodes;        //graph keeps data in map - for fast acces by key
public:
    Graph(){}
    Graph(const Ref<const MatrixXi> &matrix);
    Graph(int node_number);
    void setEdge(int node1, int node2, int edge_weigth);
    operator string() const;
    friend ostream &operator<<(ostream &os, const Graph &b);
    void addNode(int node_number, int node_colour);
    string getCanonCode();        // it's our heart of our algorithm
};

#endif // GRAPH_H
