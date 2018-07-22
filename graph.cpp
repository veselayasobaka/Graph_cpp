#include "graph.h"
//This constructor transormate squared adjacency matrix of graph
//with no more than one cycle into the adjacency list
string get_code(vector<Vertex> v);
string get_code(vector<Vertex> v, vector<Vertex> v_cycle);
string get_cycle_code(vector<Vertex> &v,vector<Vertex>::iterator iter);
bool is_equal(const Vertex &v1, const Vertex &v2)
{
    return (v1.level == v2.level)&&(v1.colour==v2.colour)&&(v1.child_range == v2.child_range)&&
            (v1.bounds.begin()->second == v2.bounds.begin()->second);
}
string code_to_string(vector<Vertex> &v)
{
    string canon_code;
    for (auto const &vertex: v)
    {
        canon_code+=to_string(vertex.level)+to_string(vertex.colour)+vertex.child_range
                +to_string(vertex.bounds.begin()->second);
    }
    return canon_code;
}

Graph:: Graph(const Ref<const MatrixXf> &m1)
{
    if (m1 == m1.transpose() && (m1.rows())*2 >= m1.count())
    {
        for (int i = 0; i < m1.rows(); ++i)
        {
            Vertex v(i);
            for (int j = 0; j < m1.rows(); ++j)
            {
                if (m1(i, j) != 0)
                {
                    v.add_bound(j, m1(i, j));
                }
            }

            vertexes.insert({i,v});
        }
    }
    else
    {
        cout << "Wrong Matrix!" << endl;
    }
}
// DON'T TRY TO UNDERSTEND IT, YOU MIND WILL CRUSH
string Graph::get_canon_code()
{
    queue<int> v_list;                      // queue for numbers of lower branches
    int a = 0;
    vector<Vertex> v_sorted;                // vector for sorted by level vertexes
    int j = 0;
    uint size = 0;              //  to leave the circle in case of cycle
    while(vertexes.size()>2)    // if graph hasn't the cycle we leave it when it only has root(s)
    {
        for(auto it = vertexes.begin(); it != vertexes.end(); ++it)
        {
            if (it->second.bounds.size() == 1)  // if our vertex have only 1 bound - it's lower branch
            {
                it->second.level = j;                                // let's write it's level
                v_sorted.push_back(it->second);                      //and take it to better place
                v_list.push(it->first);                              // remember number, it will be deleted after cycle
            }
        }
        while(v_list.size())
        {
            a = v_list.front();
            int bnd_vertex = vertexes.find(a)->second.bounds.begin()->first; // number of vertex, with witch we were connected
            auto iter = vertexes.find(bnd_vertex);               // find bounded vertex
            iter->second.bounds.erase(vertexes.find(a)->first); // erase connection with our vertex. now it have only
            vertexes.erase(a);                                   //connection - with upper branch
            v_list.pop();
        }
        ++j;
        if(size == vertexes.size())    // if graph has a cycle we leave it when
            break;                     //graph has only cycle so size stop changes
        size = vertexes.size();
    }


    if (vertexes.size()==2)
    {
        vertexes.begin()->second.level = j;
        vertexes.rbegin()->second.level = j;
        v_sorted.push_back(vertexes.begin()->second);
        v_sorted.push_back(vertexes.rbegin()->second);
        vertexes.clear();
    }
    else if (vertexes.size()==1)
    {
        vertexes.begin()->second.level = j;
        v_sorted.push_back(vertexes.begin()->second);
        vertexes.clear();
    }
    else
    {
        vector<Vertex> v_cycle;
        for (auto &vertex: vertexes)
        {
            vertex.second.level = (*v_sorted.rbegin()).level+1;
            v_cycle.push_back(vertex.second);
        }
        vertexes.clear();
        return get_code(v_sorted, v_cycle);
    }
        return get_code(v_sorted);
}

bool sortlevels(Vertex &v1, Vertex v2)
{
    return (v1.level < v2.level);
}
bool waytosort(Vertex &v1, Vertex &v2)
{
    return (v1.level < v2.level) ||
           ((v1.level == v2.level) && (v1.bounds.begin()->second < v2.bounds.begin()->second)) ||
           ((v1.level == v2.level) && (v1.bounds.begin()->second == v2.bounds.begin()->second) && (v1.colour < v2.colour)) ||
           ((v1.level == v2.level) && (v1.bounds.begin()->second == v2.bounds.begin()->second) && (v1.colour == v2.colour) &&
           (v1.child_range < v2.child_range)) ;
}
bool Graph::is_cycle()
{
    int number_bounds = 0;
    int number_nodes = (vertexes.rbegin()->second.number) + 1;
    for (auto const &vertex: vertexes)
    {
        number_bounds += vertex.second.bounds.size();
    }
    return number_bounds/2!=number_nodes-1;
}

string get_code(vector<Vertex> v)      // compute code for non-cycled graph (tree)
{
    int j = 0;       // this number will keep range of vertex
    int b = 0;       // this number will keep number of connected vertex from higher level
    int max_level = v.rbegin()->level;     // how many cycles we need to end canoniztion procedure
    auto iter2 = v.begin();               // iter2 will point at start of sort procedure of inner "for"
    for (int i = 1; i < max_level + 2; ++i)      // in order to lower number of sorting vertexes
    {
        sort(iter2, v.end(), waytosort);   // sorting part of v_sorted in which we wasn't
        for( ; (iter2->level!=i) && (iter2!=v.end()); ++iter2)  // at first time we looking at 0-levels, then
        {                                                             //at 1-s and so on until the end
            if (is_equal((*iter2), *(iter2+1)))
            {
                iter2->level = j;            // if our vertex equal to next, number of the next vertex will be the same
            }
            else
            {
                iter2->level = j;       // in other case j++
                ++j;
            }
            b = iter2->bounds.begin()->first;                        // found connected vertex from
            auto iter_find = find(iter2, v.end(), Vertex(b));        //the next level of tree
            if(iter_find == v.end())                          // if we cannot find it break the cycle
                break;
            iter_find->child_range+=to_string(iter2->level)+"_";                    // in opposite case write our range to it's child_range
        }

    }
    /*for (auto &vertex: v)
    {
        cout << vertex << endl;
    }*/

    return code_to_string(v);
}

//--------->simillar function but made to work with 1-cycled trees!<------------//

string get_code(vector<Vertex> v, vector<Vertex> v_cycle)
{
    Vertex temp(0);
    for (auto const &vertex: v_cycle)     //
    {
        temp = vertex;
        temp.bounds.clear();
        v.push_back(temp);
    }

    string code_of_cycle = "";       // for part of canonization code from cycle;


    int j = 0;       // this number will keep range of vertex
    int b = 0;       // this number will keep number of connected vertex from higher level
    int max_level = v_cycle.rbegin()->level;     // how many cycles we need to end canoniztion procedure for tree
    auto iter2 = v.begin();               // iter2 will point at start of sort procedure of inner "for"
    for (int i = 1; i < max_level + 2; ++i)      // in order to lower number of sorting vertexes
    {
        sort(iter2, v.end(), waytosort);   // sorting part of v_sorted in which we were not
        for( ; iter2->bounds.size() != 0; ++iter2)  // at first time we looking at 0-levels, then
        {                                                             //at 1-s and so on until the start of cycle vertexes
            if (is_equal((*iter2), *(iter2+1)))
            {
                iter2->level = j;            // if our vertex equal to next, number of the next vertex will be the same
            }
            else
            {
                iter2->level = j;       // in other case j++
                ++j;
            }
            b = iter2->bounds.begin()->first;                        // found connected vertex from
            auto iter_find = find(iter2, v.end(), Vertex(b));        //the next level of tree
            if(iter_find == v.end())                          // if we cannot find it break the cycle
                break;
            iter_find->child_range+=to_string(iter2->level)+"_";      // in opposite case write our range to it's child_range
        }
    }


    for (auto it = v.rbegin(); it->bounds.size()==0; ++it)     // transporting child_ranges back to v_cycle
    {
        auto iter_find = find(v_cycle.begin(), v_cycle.end(), Vertex(it->number));
        iter_find->child_range += it->child_range;
        v.erase(v.end()-1);                          // erase last element. we don't need it anymore
    }

    sort(v_cycle.begin(), v_cycle.end(), waytosort);
    j = max_level;                                  // numerate from the first level in cycle
    for (auto iter = v_cycle.begin(); iter != v_cycle.end(); ++iter)
    {
        if (is_equal((*iter), *(iter+1)))
        {
            iter->level = j;
        }
        else
        {
            iter->level = j;
            ++j;
        }

    }

    if (v_cycle.begin()->level != (v_cycle.begin() + 1)->level)    // simple(haha) case when we have one vertex in cycle
    {
        code_of_cycle = get_cycle_code(v_cycle, v_cycle.begin());
    }
    else                           // and if we have number of vertexes with same level, colour, etc?
    {
        for (auto iter = v_cycle.begin(); iter != v_cycle.end(); ++iter)
            cout << get_cycle_code(v_cycle, iter) << endl;

    }
    for (auto const &vert:v_cycle)
    {
        cout << vert << endl;
    }
    cout << code_of_cycle + "C" + code_to_string(v) << endl;
    return code_of_cycle + "C" + code_to_string(v);
}


    //CANONIZATION CYCLE PROCEDURE(from pointed vertex)! WARNING, HARD TO UNDERSTAND
string get_cycle_code(vector<Vertex> &v_cycle,vector<Vertex>::iterator iter_in)
{
    string cycle_left = "";                // strings for cononization code of cycle if going left
    string cycle_right = "";               //and right
    int bnd_vertex = 0;                    // number for keeping number of bounded vertex with lower number than others
    int previous_vertex = -1;              // number for keeping number of previous vertex (don't want to go back-forwards)
    auto iter = iter_in;

    for (uint k = 0; k < v_cycle.size(); ++k)                  // code for moving to the left
    {
        cycle_right+=to_string(iter->level)+to_string(iter->colour)+iter->child_range
                +to_string(iter->bounds.begin()->second);      // writing all meaning numbers in string
        if (iter->bounds.begin()->first!=previous_vertex)     // testing number of previous vortex
        {
            bnd_vertex = iter->bounds.begin()->first;         // where we going to
            previous_vertex = iter->number;                   // remember the number for next cycle
            iter = find(v_cycle.begin(), v_cycle.end(), Vertex(bnd_vertex));       // finding vertex where we going to
        }
        else
        {
            bnd_vertex = iter->bounds.rbegin()->first;       // the same, in case when we came
            previous_vertex = iter->number;                  //from vertex with same number
            iter = find(v_cycle.begin(), v_cycle.end(), Vertex(bnd_vertex));
        }
    }
    previous_vertex = -1;                  // the simmilar case for going through cycle starting from other direction
    iter = iter_in;
    for (uint k = 0; k < v_cycle.size(); ++k)
    {
        cycle_left+=to_string(iter->level)+to_string(iter->colour)+iter->child_range
                +to_string(iter->bounds.rbegin()->second);
        if (k == 0)                        // the only change - starting in opposite direction
        {
            bnd_vertex = iter->bounds.rbegin()->first;
            previous_vertex = iter->number;
            iter = find(v_cycle.begin(), v_cycle.end(), Vertex(bnd_vertex));
        }
        else if (iter->bounds.begin()->first!=previous_vertex)
        {
            bnd_vertex = iter->bounds.begin()->first;
            previous_vertex = iter->number;
            iter = find(v_cycle.begin(), v_cycle.end(), Vertex(bnd_vertex));
        }
        else
        {
            bnd_vertex = iter->bounds.rbegin()->first;
            previous_vertex = iter->number;
            iter = find(v_cycle.begin(), v_cycle.end(), Vertex(bnd_vertex));
        }
    }

    return min(cycle_left, cycle_right);
}

