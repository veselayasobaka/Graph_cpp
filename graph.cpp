#include "graph.h"

string getCode(vector<Node> v);     // prototypes
string getCode(vector<Node> v, vector<Node> v_cycle);
string getCycleCode(vector<Node> &v,vector<Node>::iterator iter);

// metods connected with nodes
void Node::addEdge(int a, int b)
{
    edges.insert({a,b});
}
Node::operator string() const
    {
        string a = ("") + to_string(number) + " " + to_string(level) + " "
                + to_string(colour) + " " + child_range + "           ";
        for(auto it = edges.begin(); it != edges.end(); ++it)
        {
            a += "(" + to_string(it->first) + ", " + to_string(it->second) + ")";
        }
        return a;
    }
ostream& operator<<(ostream &os, const Node& b)
{
    return os << string(b);
}
bool operator==(const Node &vl, const Node &vr)
{
    return vl.number == vr.number;
}
bool isEqual(const Node &v1, const Node &v2)           // function to test equivalence of _nodes
{
    return (v1.level == v2.level)&&(v1.colour==v2.colour)&&(v1.child_range == v2.child_range)&&
            (v1.edges.begin()->second == v2.edges.begin()->second);
}
string codeTostring(vector<Node> &v)                             // Generates code of tree
{                                                                    //if tree is connonized - it is canonical code
    string canon_code;
    for (auto const &node: v)
    {
        canon_code+=to_string(node.level)+to_string(node.colour)+node.child_range
                +to_string(node.edges.begin()->second);
    }
    return canon_code;
}

//metods connected with Graphs
//This constructor transforms squared adjacency matrix of graph
//with no more than one cycle into the adjacency list
Graph:: Graph(const Ref<const MatrixXi> &m1)
{
    if (m1 == m1.transpose() && (m1.rows())*2 >= m1.count())
    {
        for (int i = 0; i < m1.rows(); ++i)
        {
            Node v(i);
            for (int j = 0; j < m1.rows(); ++j)
            {
                if (m1(i, j) != 0)
                {
                    v.addEdge(j, m1(i, j));
                }
            }

            _nodes.insert({i,v});
        }
    }
    else
    {
        cout << "Wrong Matrix!" << endl;
    }
}
Graph::Graph(int q)
{
    for (int i = 0; i < q; ++i)
    {
        Node v(i);
        _nodes.insert({i, v});
    }
}

void Graph::setEdge(int a, int b, int c)      //set edge between a and b nodes with weight c
{
    auto it = _nodes.find(a);
    it->second.addEdge(b, c);
    it = _nodes.find(b);
    it->second.addEdge(a, c);
}
void Graph::addNode(int a, int b)
    {
        Node v(a, b);
        _nodes.insert({a, v});
    }
Graph::operator string() const
{
    string a;
    for(auto it = _nodes.begin(); it != _nodes.end(); ++it)
    {

        a+=it->second;
        a+="\n";
    }
    return a;
}
ostream& operator<<(ostream &os, const Graph& b)
    {
        return os << string(b);
    }
bool waytosort(Node &v1, Node &v2)    // sorts by all meaning parametres
{
    return (v1.level < v2.level) ||
           ((v1.level == v2.level) && (v1.edges.begin()->second < v2.edges.begin()->second)) ||
           ((v1.level == v2.level) && (v1.edges.begin()->second == v2.edges.begin()->second) && (v1.colour < v2.colour)) ||
           ((v1.level == v2.level) && (v1.edges.begin()->second == v2.edges.begin()->second) && (v1.colour == v2.colour) &&
           (v1.child_range < v2.child_range)) ;
}
bool Graph::isCycle()          // can test is graph cycle or not
{
    int number_edges = 0;
    int number_nodes = (_nodes.rbegin()->second.number) + 1;
    for (auto const &node: _nodes)
    {
        number_edges += node.second.edges.size();
    }
    return number_edges/2!=number_nodes-1;
}
// function that takes graph, distributes it _nodes by levels and gives you
//canonical graph code with a little help of getCode function
// DON'T TRY TO UNDERSTEND IT, YOU MIND WILL CRUSH
string Graph::getCanonCode()
{
    queue<int> v_list;                      // queue for numbers of lower branches
    int a = 0;
    vector<Node> v_sorted;                // vector for sorted by level _nodes
    int j = 0;
    uint size = 0;              //  to leave the circle in case of cycle
    while(_nodes.size()>2)    // if graph hasn't the cycle we leave it when it only has root(s)
    {
        for(auto it = _nodes.begin(); it != _nodes.end(); ++it)
        {
            if (it->second.edges.size() == 1)  // if our node have only 1 edge - it's lower branch
            {
                it->second.level = j;                                // let's write it's level
                v_sorted.push_back(it->second);                      //and take it to better place
                v_list.push(it->first);                              // remember number, it will be deleted after cycle
            }
        }
        while(v_list.size())
        {
            a = v_list.front();
            int bnd_node = _nodes.find(a)->second.edges.begin()->first; // number of node, with witch we were connected
            auto iter = _nodes.find(bnd_node);               // find edgeed node
            iter->second.edges.erase(_nodes.find(a)->first); // erase connection with our node. now it have only
            _nodes.erase(a);                                   //connection - with upper branch
            v_list.pop();
        }
        ++j;
        if(size == _nodes.size())    // if graph has a cycle we leave it when
            break;                     //graph has only cycle so size stop changes
        size = _nodes.size();
    }


    if (_nodes.size()==2)
    {
        _nodes.begin()->second.level = j;
        _nodes.rbegin()->second.level = j;
        v_sorted.push_back(_nodes.begin()->second);
        v_sorted.push_back(_nodes.rbegin()->second);
        _nodes.clear();
    }
    else if (_nodes.size()==1)
    {
        _nodes.begin()->second.level = j;
        v_sorted.push_back(_nodes.begin()->second);
        _nodes.clear();
    }
    else
    {
        vector<Node> v_cycle;
        for (auto &node: _nodes)
        {
            node.second.level = (*v_sorted.rbegin()).level+1;
            v_cycle.push_back(node.second);
        }
        _nodes.clear();
        return getCode(v_sorted, v_cycle);
    }
        return getCode(v_sorted);
}

string getCode(vector<Node> v)      // compute code for non-cycled graph (tree)
{
    int j = 0;       // this number will keep range of node
    int b = 0;       // this number will keep number of connected node from higher level
    int max_level = v.rbegin()->level;     // how many cycles we need to end canoniztion procedure
    auto iter2 = v.begin();               // iter2 will point at start of sort procedure of inner "for"
    for (int i = 1; i < max_level + 2; ++i)      // in order to lower number of sorting _nodes
    {
        sort(iter2, v.end(), waytosort);   // sorting part of v_sorted in which we wasn't
        for( ; (iter2->level!=i) && (iter2!=v.end()); ++iter2)  // at first time we looking at 0-levels, then
        {                                                             //at 1-s and so on until the end
            if (isEqual((*iter2), *(iter2+1)))
            {
                iter2->level = j;            // if our node equal to next, number of the next node will be the same
            }
            else
            {
                iter2->level = j;       // in other case j++
                ++j;
            }
            b = iter2->edges.begin()->first;                        // found connected node from
            auto iter_find = find(iter2, v.end(), Node(b));        //the next level of tree
            if(iter_find == v.end())                          // if we cannot find it break the cycle
                break;
            iter_find->child_range+=to_string(iter2->level)+"_";                    // in opposite case write our range to it's child_range
        }

    }

    return codeTostring(v);
}

//--------->simillar function but made to work with 1-cycled trees!<------------//

string getCode(vector<Node> v, vector<Node> v_cycle)
{
    Node temp(0);
    for (auto const &node: v_cycle)     //
    {
        temp = node;
        temp.edges.clear();
        v.push_back(temp);
    }

    string code_of_cycle = "";       // for part of canonization code from cycle;


    int j = 0;       // this number will keep range of node
    int b = 0;       // this number will keep number of connected node from higher level
    int max_level = v_cycle.rbegin()->level;     // how many cycles we need to end canoniztion procedure for tree
    auto iter2 = v.begin();               // iter2 will point at start of sort procedure of inner "for"
    for (int i = 1; i < max_level + 2; ++i)      // in order to lower number of sorting _nodes
    {
        sort(iter2, v.end(), waytosort);   // sorting part of v_sorted in which we were not
        for( ; iter2->edges.size() != 0; ++iter2)  // at first time we looking at 0-levels, then
        {                                                             //at 1-s and so on until the start of cycle _nodes
            if (isEqual((*iter2), *(iter2+1)))
            {
                iter2->level = j;            // if our node equal to next, number of the next node will be the same
            }
            else
            {
                iter2->level = j;       // in other case j++
                ++j;
            }
            b = iter2->edges.begin()->first;                        // found connected node from
            auto iter_find = find(iter2, v.end(), Node(b));        //the next level of tree
            if(iter_find == v.end())                          // if we cannot find it break the cycle
                break;
            iter_find->child_range+=to_string(iter2->level)+"_";      // in opposite case write our range to it's child_range
        }
    }


    for (auto it = v.rbegin(); it->edges.size()==0; ++it)     // transporting child_ranges back to v_cycle
    {
        auto iter_find = find(v_cycle.begin(), v_cycle.end(), Node(it->number));
        iter_find->child_range += it->child_range;
        v.erase(v.end()-1);                          // erase last element. we don't need it anymore
    }

    sort(v_cycle.begin(), v_cycle.end(), waytosort);
    j = max_level;                                  // numerate from the first level in cycle
    for (auto iter = v_cycle.begin(); iter != v_cycle.end(); ++iter)
    {
        if (isEqual((*iter), *(iter+1)))
        {
            iter->level = j;
        }
        else
        {
            iter->level = j;
            ++j;
        }

    }

    if (v_cycle.begin()->level != (v_cycle.begin() + 1)->level)    // simple(haha) case when we have one node in cycle
    {
        code_of_cycle = getCycleCode(v_cycle, v_cycle.begin());
    }
    else                           // and if we have number of _nodes with same level, colour, etc?
    {
        code_of_cycle = getCycleCode(v_cycle, v_cycle.begin());
        int level_of_first = v_cycle.begin()->level;
        for (auto iter = v_cycle.begin() + 1; iter->level == level_of_first; ++iter)
        {
            code_of_cycle = min(code_of_cycle, getCycleCode(v_cycle, iter));
        }
    }
    return codeTostring(v) + "C" + code_of_cycle;
}


    //CANONIZATION CYCLE PROCEDURE(from pointed node)! WARNING, HARD TO UNDERSTAND
string getCycleCode(vector<Node> &v_cycle,vector<Node>::iterator iter_in)
{
    string cycle_left = "";                // strings for cononization code of cycle if going left
    string cycle_right = "";               //and right
    int bnd_node = 0;                    // number for keeping number of edgeed node with lower number than others
    int previous_node = -1;              // number for keeping number of previous node (don't want to go back-forwards)
    auto iter = iter_in;

    for (uint k = 0; k < v_cycle.size(); ++k)                  // code for moving to the left
    {
        cycle_right+=to_string(iter->level)+to_string(iter->colour)+iter->child_range
                +to_string(iter->edges.begin()->second);      // writing all meaning numbers in string
        if (iter->edges.begin()->first!=previous_node)     // testing number of previous vortex
        {
            bnd_node = iter->edges.begin()->first;         // where we going to
            previous_node = iter->number;                   // remember the number for next cycle
            iter = find(v_cycle.begin(), v_cycle.end(), Node(bnd_node));       // finding node where we going to
        }
        else
        {
            bnd_node = iter->edges.rbegin()->first;       // the same, in case when we came
            previous_node = iter->number;                  //from node with same number
            iter = find(v_cycle.begin(), v_cycle.end(), Node(bnd_node));
        }
    }
    previous_node = -1;                  // the simmilar case for going through cycle starting from other direction
    iter = iter_in;
    for (uint k = 0; k < v_cycle.size(); ++k)
    {
        cycle_left+=to_string(iter->level)+to_string(iter->colour)+iter->child_range
                +to_string(iter->edges.rbegin()->second);
        if (k == 0)                        // the only change - starting in opposite direction
        {
            bnd_node = iter->edges.rbegin()->first;
            previous_node = iter->number;
            iter = find(v_cycle.begin(), v_cycle.end(), Node(bnd_node));
        }
        else if (iter->edges.begin()->first!=previous_node)
        {
            bnd_node = iter->edges.begin()->first;
            previous_node = iter->number;
            iter = find(v_cycle.begin(), v_cycle.end(), Node(bnd_node));
        }
        else
        {
            bnd_node = iter->edges.rbegin()->first;
            previous_node = iter->number;
            iter = find(v_cycle.begin(), v_cycle.end(), Node(bnd_node));
        }
    }

    return min(cycle_left, cycle_right);
}

