#include "graph.h"
using namespace std;
using namespace Eigen;
Graph read_file(const string &s);
int main(int argc, char* argv[])
{
    MatrixXf m1(10, 10);
    m1 <<   0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 0, 0, 0, 0, 0,
            1, 0, 0, 23, 1, 0, 0, 0, 0, 0,
            0, 1, 23, 0, 0, 0, 12, 0, 0, 0,
            0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 4, 0, 0, 0,
            0, 0, 0, 12, 0, 4, 0, 0, 0, 5,
            0, 0, 0, 0, 0, 0, 0, 0, 3, 0,
            0, 0, 0, 0, 0, 0, 0, 3, 0, 1,
            0, 0, 0, 0, 0, 0, 5, 0, 1, 0;



    Graph g1(m1);
    g1 = read_file(argv[1]);
    g1.set_levels();
    cout << g1;
    return 0;
}


