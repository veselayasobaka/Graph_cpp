#include <algorithm>
#include <vector>
#include <queue>
#include "graph.h"
using namespace std;
string getCode(vector<Node> v);                                     // prototypes
string getCode(vector<Node> v, vector<Node> nodes_cycle);
string getCycleCode(vector<Node> &v,vector<Node>::iterator iter);

/**
 * \brief metods connected with nodes
 * \param [in] node_number -
 * \param [in] edge_weight -
 * \return Nothing
 */
void Node::addEdge(int node_number, int edge_weigth)
{
    _edges.insert({node_number, edge_weigth});
}

/**
 * \brief type casting to std::string
 * \return string
 */
Node::operator string() const
    {
        string node_str = ("") + to_string(_number) + " " + to_string(_level) + " "
                + to_string(_colour) + " " + _child_range + "           ";
        for(auto it = _edges.begin(); it != _edges.end(); ++it)
        {
            node_str += "(" + to_string(it->first) + ", " + to_string(it->second) + ")";
        }
        return node_str;
    }

ostream& operator<<(ostream &os, const Node& str)
{
    return os << string(str);
}
bool operator==(const Node &vl, const Node &vr)
{
    return vl._number == vr._number;
}
bool isEqual(const Node &v1, const Node &v2)                     // function to test equivalence of _nodes
{
    return (v1._level == v2._level)&&(v1._colour==v2._colour)&&(v1._child_range == v2._child_range)&&
            (v1._edges.begin()->second == v2._edges.begin()->second);
}
string codeTostring(vector<Node> &v)                             // Generates code of tree
{                                                                //if tree is connonized - it'll be a canonical code
    string canon_code;
    for (auto const &node: v)
    {
        canon_code+="("+to_string(node._level)+to_string(node._colour)+node._child_range
                +to_string(node._edges.begin()->second)+")";
    }
    return canon_code;
}

//metods connected with Graphs

Graph::Graph(int number_nodes)                                  // gives Graph for given number of nodes (without bounds)
{
    for (int i = 0; i < number_nodes; ++i)
    {
        Node v(i);
        _nodes.insert({i, v});
    }
}

void Graph::setEdge(int node1, int node2, int edge_weigth)      //set edge between node1 and node2 with edge_weigth
{
    auto it = _nodes.find(node1);
    it->second.addEdge(node2, edge_weigth);
    it = _nodes.find(node2);
    it->second.addEdge(node1, edge_weigth);
}
void Graph::addNode(int node_number, int node_colour)
    {
        Node v(node_number, node_colour);
        _nodes.insert({node_number, v});
    }
Graph::operator string() const
{
    string graph_str;
    for(auto it = _nodes.begin(); it != _nodes.end(); ++it)
    {

        graph_str += it->second;
        graph_str += "\n";
    }
    return graph_str;
}
ostream& operator<<(ostream &os, const Graph& gstr)
{
    return os << string(gstr);
}
bool waytosort(Node &v1, Node &v2)                              // sorts by all meaning parametres
{
    return (v1._level < v2._level) ||
           ((v1._level == v2._level) && (v1._edges.begin()->second < v2._edges.begin()->second)) ||
           ((v1._level == v2._level) && (v1._edges.begin()->second == v2._edges.begin()->second) &&
           (v1._colour < v2._colour)) ||
           ((v1._level == v2._level) && (v1._edges.begin()->second == v2._edges.begin()->second) &&
           (v1._colour == v2._colour) && (v1._child_range < v2._child_range));
}

// function that takes graph, distributes it _nodes by levels and gives you
//canonical graph code with a little help of getCode function
// DON'T TRY TO UNDERSTEND IT, YOU MIND WILL CRUSH
string Graph::getCanonCode()
{
    queue<int> queue_n_nodes;                   //queue for numbers of lower branches
    vector<Node> node_sorted;                   // vector for sorted by level nodes
    int current_level = 0;                      // current level to write in level of node
    size_t size = 0;                            //  to leave the circle in case of cycle
    while(_nodes.size()>2)                      // if graph hasn't the cycle we leave it when it only has root(s)
    {
        for(auto it = _nodes.begin(); it != _nodes.end(); ++it)
        {
            if (it->second._edges.size() == 1)  // if our node have only 1 edge - it's lower branch
            {
                it->second._level = current_level;             // let's write it's level
                node_sorted.push_back(it->second);             //and take it to better place
                queue_n_nodes.push(it->first);                 // remember number, it will be deleted after cycle
            }
        }
        while(queue_n_nodes.size())
        {
            const auto node_to_del = queue_n_nodes.front();
            int bnd_node = _nodes.find(node_to_del)->second._edges.begin()->first; // number of node edged with us
            auto iter = _nodes.find(bnd_node);                                     // find edgeed node
            iter->second._edges.erase(_nodes.find(node_to_del)->first); // erase connection with our node. now it have
            _nodes.erase(node_to_del);                                  //the only connection - with upper branch
            queue_n_nodes.pop();
        }
        ++current_level;
        if(size == _nodes.size())      // if graph has a cycle we leave it when
            break;                     //graph has only cycle so size stop changes
        size = _nodes.size();          //size == previous size. so we can see is there a changes
    }


    if (_nodes.size()==2)
    {
        _nodes.begin()->second._level = current_level;      // if size of _nodes == 2, than we have tree wirh edge on
        _nodes.rbegin()->second._level = current_level;     //the top. setting numbers for both of roots
        node_sorted.push_back(_nodes.begin()->second);      //than push it to nodes_sorted
        node_sorted.push_back(_nodes.rbegin()->second);
        _nodes.clear();
    }
    else if (_nodes.size()==1)
    {
        _nodes.begin()->second._level = current_level;      // simillar case, but we have the only root
        node_sorted.push_back(_nodes.begin()->second);
        _nodes.clear();
    }
    else                                                    // size of nodes_sorted stopped changing, but we have more
    {                                                       //than two root on top. this means we have a cycled graph
        vector<Node> nodes_cycle;                           //copy it's nodes to vector nodes_cycle. we will keep
        for (auto &node: _nodes)                            // working with this vector, so whipe previous one
        {
            node.second._level = (*node_sorted.rbegin())._level+1;
            nodes_cycle.push_back(node.second);
        }
        _nodes.clear();
        return getCode(node_sorted, nodes_cycle);           // with cycle
    }
        return getCode(node_sorted);                        //without
}

string getCode(vector<Node> v)      // compute code for non-cycled graph (tree)
{
    int current_level = 0;          // this number will keep current level of node
    const auto max_level = v.rbegin()->_level;     // how many cycles we need to end canoniztion procedure
    auto iter = v.begin();                         // iter will point at start of sort procedure of inner "for"
    for (int i = 1; i < max_level + 2; ++i)        // in order to lower number of sorting _nodes
    {
        sort(iter, v.end(), waytosort);            // sorting part of node_sorted in which we wasn't
        for( ; (iter->_level!=i) && (iter!=v.end()); ++iter)  // at first time we looking at 0-levels, then
        {                                                     //at 1-s and so on until the end
            iter->_level = current_level;          // if our node equal to next, number of next node will be the same
            if (!isEqual((*iter), *(iter+1)))
            {
                ++current_level;                   // in other case current_level++
            }
            const auto n_node = iter->_edges.begin()->first;                   // let's find connected node from
            auto iter_find = find(iter, v.end(), Node(n_node));                //the next level of tree
            if(iter_find == v.end())                                           // if we cannot find it break the cycle
                break;
            iter_find->_child_range+= "_" + to_string(iter->_level)+ "_";      // in opposite case write our range
        }                                                                      //to it's child_range

    }

    return codeTostring(v);
}

//--------->simillar function but made to work with 1-cycled trees!<------------//

string getCode(vector<Node> v, vector<Node> nodes_cycle)
{

    Node temp(0);
    for (auto const &node: nodes_cycle)     //
    {
        temp = node;
        temp._edges.clear();
        v.push_back(temp);
    }


    int current_level = 0;                         // this number will keep range of node
    int max_level = nodes_cycle.rbegin()->_level;  // how many cycles we need to end canoniztion procedure for tree
    auto iter = v.begin();                         // iter will point at start of sort procedure of inner "for"
    for (int i = 1; i < max_level + 2; ++i)        // in order to lower number of sorting _nodes
    {
        sort(iter, v.end(), waytosort);            // sorting part of node_sorted in which we were not
        for( ; iter->_edges.size() != 0; ++iter)   // at first time we looking at 0-_levels, then
        {                                          //at 1-s and so on until the start of cycle _nodes
            iter->_level = current_level;          // if our node equal to next,
            if (!isEqual((*iter), *(iter+1)))      //number of the next node will be the same
            {
                ++current_level;                   // in other case current_level++
            }
            const auto n_node = iter->_edges.begin()->first;          // found connected node from
            auto iter_find = find(iter, v.end(), Node(n_node));       //the next level of tree
            if(iter_find == v.end())                                  // if we cannot find it break the cycle
                break;
            iter_find->_child_range+= + "_" + to_string(iter->_level)+"_";     // in opposite case write our range
        }                                                             //to it's child_range
    }


    for (auto it = v.rbegin(); it->_edges.size()==0; ++it)            // transporting child_ranges back to nodes_cycle
    {
        auto iter_find = find(nodes_cycle.begin(), nodes_cycle.end(), Node(it->_number));
        iter_find->_child_range += it->_child_range;
        v.erase(--v.end());                                           // erase last element. we don't need it anymore
    }

    sort(nodes_cycle.begin(), nodes_cycle.end(), waytosort);
    current_level = max_level;                                        // numerate from the first level in cycle
    for (auto iter = nodes_cycle.begin(); iter != nodes_cycle.end(); ++iter)
    {
        iter->_level = current_level;
        if (!isEqual((*iter), *(iter+1)))
        {
            ++current_level;
        }
    }
    auto code_of_cycle = getCycleCode(nodes_cycle, nodes_cycle.begin());
    if (nodes_cycle.begin()->_level == (nodes_cycle.begin() + 1)->_level)    // if we have more than one minimal
    {                                                                        //node in cycle
        const auto level_of_first = nodes_cycle.begin()->_level;             // remember this level
        for (auto iter = nodes_cycle.begin() + 1; iter->_level == level_of_first; ++iter) // compute string for all
        {                                                                                 //minimal nodes
            code_of_cycle = min(code_of_cycle, getCycleCode(nodes_cycle, iter));
        }
    }
    return codeTostring(v) + "C" + code_of_cycle;
}


    //CANONIZATION CYCLE PROCEDURE(from pointed node)! WARNING, HARD TO UNDERSTAND
string getCycleCode(vector<Node> &nodes_cycle,vector<Node>::iterator iter_in)
{
    string cycle_left = "";              // strings for cononization code of cycle if going left
    string cycle_right = "";             //and right
    int bnd_node = 0;                    // number for keeping number of edgeed node with lower number than others
    int previous_node = -1;              // number for keeping number of previous node (don't want to go back-forwards)
    auto iter = iter_in;
    for (size_t k = 0; k < nodes_cycle.size(); ++k)                  // code for moving to the left
    {
        if (iter->_edges.begin()->first!=previous_node)              // testing number of previous vortex
        {
            bnd_node = iter->_edges.begin()->first;                  // where we going to
            cycle_right += "(" + to_string(iter->_colour) + iter->_child_range
                    + to_string(iter->_edges.begin()->second) + ")";            // writing all meaning numbers in string
        }
        else
        {
            bnd_node = iter->_edges.rbegin()->first;                 // the same, in case when we came
            cycle_right+="(" + to_string(iter->_colour)+iter->_child_range
                    +to_string(iter->_edges.rbegin()->second) + ")";            // writing all meaning numbers in string
        }
        previous_node = iter->_number;                               // remember our number for next cycle
        iter = find(nodes_cycle.begin(), nodes_cycle.end(), Node(bnd_node));     // finding node where we going to
    }
    previous_node = -1;                 // the simmilar case for going through cycle starting from other direction
    iter = iter_in;                     // and we need to return iter and previous node to the beginnig state
    for (size_t k = 0; k < nodes_cycle.size(); ++k)
    {
        if (k == 0)                     // the only change - starting in opposite direction
        {
            bnd_node = iter->_edges.rbegin()->first;
            cycle_left+="(" + to_string(iter->_colour)+iter->_child_range
                    +to_string(iter->_edges.rbegin()->second) + ")";
        }
        else if (iter->_edges.begin()->first!=previous_node)
        {
            bnd_node = iter->_edges.begin()->first;
            cycle_left+="(" + to_string(iter->_colour)+iter->_child_range
                    +to_string(iter->_edges.begin()->second) + ")";
        }
        else
        {
            bnd_node = iter->_edges.rbegin()->first;
            cycle_left+="(" + to_string(iter->_colour)+iter->_child_range
                    +to_string(iter->_edges.rbegin()->second) + ")";
        }
        previous_node = iter->_number;
        iter = find(nodes_cycle.begin(), nodes_cycle.end(), Node(bnd_node));
    }

    return min(cycle_left, cycle_right);
}

