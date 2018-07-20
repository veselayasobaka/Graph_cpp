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
            vertexes.push_back(v);
        }
    }
    else
    {
        cout << "Wrong Matrix!" << endl;
    }
}

void Graph::set_levels()
{


}

bool sortlevels(Vertex &v1, Vertex v2)
{
    return (v1.level < v2.level);
}


