#include "graph.h"
//This constructor transormate squared adjacency matrix of graph
//with no more than one cycle into the adjacency list
Graph:: Graph(MatrixXf m1)
{
    if (m1 == m1.transpose() && (m1.rows())*2 >= m1.count())
    {
        for (int i = 0; i < m1.rows(); ++i)
        {
            Vortex v(i);
            for (int j = 0; j < m1.rows(); ++j)
            {
                if (m1(i, j) != 0)
                {
                    Bound b1(j, m1(i, j));
                    v.add_bound(b1);
                }
            }
            vortexes.push_back(v);
        }
    }
    else
    {
        cout << "Wrong Matrix!" << endl;
    }
}

void Graph::set_levels()
{
    for (int j = 1; j < 2; ++j)
    {

        for (auto it = vortexes.begin(); it != vortexes.end(); ++it)
        {
            if (it->bounds.size()+it->level == 1)
            {
                it->level = j;
                int bnd_vortex = it->bounds[0].number_vortex;
                --vortexes[bnd_vortex].level;
                vortexes[bnd_vortex].move_bound_to_end(it->number, it->bounds[0].weigth);
            }
        }
        sort(vortexes.begin(), vortexes.end(), sortlevels);
    }
}

bool waytosort(Vortex v1, Vortex v2)
{
    return (v1.level < v2.level) ||
           ((v1.level == v2.level) && (v1.bounds[0].weigth < v2.bounds[0].weigth)) ||
           ((v1.level == v2.level) && (v1.bounds[0].weigth == v2.bounds[0].weigth) && (v1.colour < v2.colour)) ||
           ((v1.level == v2.level) && (v1.bounds[0].weigth == v2.bounds[0].weigth) && (v1.colour == v2.colour) &&
           (v1.child_range < v2.child_range)) ;
}
bool sortlevels(Vortex v1, Vortex v2)
{
    return (v1.level < v2.level);
}
bool operator ==(const Bound &lhs, const Bound &rhs)
{
    return (lhs.number_vortex==rhs.number_vortex)&&(lhs.weigth==rhs.weigth);
}

