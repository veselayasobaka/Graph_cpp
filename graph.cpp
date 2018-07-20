#include "graph.h"
//This constructor transormate squared adjacency matrix of graph
//with no more than one cycle into the adjacency list
Graph:: Graph(MatrixXf &m1)
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
//DON'T TRY TO UNDERSTEND IT, YOU MIND WILL CRUSH
void Graph::set_levels()
{
    queue<int> v_list;                      //queue for numbers of lower branches
    int a = 0;
    vector<Vertex> v_sorted;                //vector for sorted by level vertexes
    int j = 0;
    while(vertexes.size()>2)
    {
        for(auto it = vertexes.begin(); it != vertexes.end(); ++it)
        {
            if (it->second.bounds.size() == 1)  //if our vertex have only 1 bound - it's lower branch
            {
                it->second.level = j;                                //let's write it's level
                v_sorted.push_back(it->second);                      //taking it to better place
                v_list.push(it->first);                              //remember number, it will be deleted after cycle
            }
        }
        while(v_list.size())
        {
            a = v_list.front();
            int bnd_vertex = vertexes.find(a)->second.bounds.begin()->first; //number of vertex, with witch we were connected
            auto iter = vertexes.find(bnd_vertex);               //find bounded vertex
            iter->second.bounds.erase(vertexes.find(a)->first); //erase connection with our vertex. now it have only
            vertexes.erase(a);                                   //connection - with upper branch
            v_list.pop();
        }
        ++j;
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
        cout << "SOMTHING GO WRONG!!!!! MORE THAN TWO ROOTS!!" << endl;
    j = 0;
    sort(v_sorted.begin(), v_sorted.end(), waytosort);
    for (auto &vertex: v_sorted)
    {

        cout << vertex << endl;
    }


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

