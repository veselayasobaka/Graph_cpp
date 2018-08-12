#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <map>

 /**
 * @brief The Node struct - structure to keep all data about some node:
 *it's number, level, colour(weight), range of it's children and vector of edges
 */
struct Node
{
    /**
     * @brief Node - constructor of node for graph
     * @param number - the number of given node
     */
    Node(int number): _number(number){}
    /**
     * @brief Node - constructor of node for graph
     * @param number - the number of given node
     * @param colour - the weight of given node
     */
    Node(int number, int colour): _number(number), _colour(colour){}
    int _number = 0;
    int _level = 0;
    int _colour = 0;
    std::string _child_range = "";
    std::map<int, int> _edges;
    /**
     * @brief metods connected with nodes
     * @param [in] node_number - the number of node from graph in integer
     * @param [in] edge_weight - the weight of given node in integer
     * @return Nothing
     */
    void addEdge(int node_number, int edge_weigth);
    /**
     * @brief operator std::string - type casting the information about Node to string
     */
    operator std::string() const;
    /**
     * @brief operator << - redirecting your node to ostream
     * @param os - where to redirect
     * @param b - choosen node
     * @return new reference to ostream with redirected node
     */
    friend std::ostream &operator<<(std::ostream &os, const Node &b);
    /**
     * @brief operator == overloading of operation ==, to support find() function 
     * @param vl - first node
     * @param vr - second node
     * @return true if nodes have same level, false in opposite case
     */
    friend bool operator==(const Node &vl, const Node &vr);
};

    /**
     * @brief waytosort - for supporting sort() function. At first compares level of node, then weight of edge
     * with the next node, then colour of nodes and at last child ranges of nodes.
     * @param v1 - first node
     * @param v2 - second node
     * @return true if first node higher then second, false in opposite
     */
bool waytosort(Node &v1, Node &v2);


// Graph - class for interaction with user.
//Graph keeps vector of _nodes and translates adjacency matrix to adjacency list(vector of _nodes).
//it's metods also cononize graph.
/**
 * @brief The Graph class for interaction with user.
 *Graph keeps vector of _nodes.
 *it's metods also cononize graph.
 */
class Graph
{
private:
    /**
     * @brief _nodes - graph keeps data in map - for fast acces by key
     */
    std::map<int, Node> _nodes;
public:
    /**
     * @brief default Graph constructor disabled for not having use
     */
    Graph(){} = delete;
    /**
     * @brief Graph constructor creates Graph for given number of nodes (without bounds) 
     * @param node_number - defines size of Graph
     */
    Graph(int node_number);
    /**
     * @brief setEdge - sets edge between node1 and node2 with edge_weigth
     * @param node1 - first node number
     * @param node2 - second node number
     * @param edge_weigth - the weight of edge
     */
    void setEdge(int node1, int node2, int edge_weigth);
    /**
     * @brief operator std::string - - type casting the information about Graph to string
     */
    operator std::string() const;
    /**
     * @brief operator << - redirecting your Graph to ostream
     * @param os - where to redirect
     * @param b - choosen Graph
     * @return new reference to ostream with redirected node
     */
    friend std::ostream &operator<<(std::ostream &os, const Graph &b);
    /**
     * @brief addNode - adds new node to graph
     * @param node_number - the number of node
     * @param node_colour - the colour of edge
     */
    void addNode(int node_number, int node_colour);
    /**
     * @brief getCanonCode - method that takes graph, distributes it _nodes by levels and gives you
     * canonical graph code with a little help of getCode function
     * @return string of canonical graph code. Example of this code:
     * (100)C(10)(10)(10_0_0) - in brackets before "C" all meaning information about nodes - level
     * of node, it's colour, child range - in case of their existing starts and ends with "_", and
     * weight of edge with upper node. After letter "C" starts cycle code. Since all cycle nodes have 
     * the same level, the level has no sense, so it starts with node colour.
     */
    std::string getCanonCode();
};

#endif // GRAPH_H
