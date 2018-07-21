#include "graph.h"
using namespace std;
using namespace Eigen;
void matrix_to_file(const Ref<const MatrixXf>& m);
Graph read_file(const string &s);
int main(int argc, char* argv[])
{
    MatrixXf m1(10, 10);
    m1 <<   0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 1, 0, 0, 0, 0, 0, 0, 0,
            1, 1, 0, 1, 0, 0, 0, 0, 0, 0,
            0, 0, 1, 0, 1, 0, 0, 0, 0, 0,
            0, 0, 0, 1, 0, 1, 0, 0, 0, 0,
            0, 0, 0, 0, 1, 0, 1, 0, 0, 0,
            0, 0, 0, 0, 0, 1, 0, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 1, 0, 1, 1,
            0, 0, 0, 0, 0, 0, 0, 1, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 1, 0, 0;

    Graph g1(m1);
    cout << g1.get_canon_code();
    return 0;
}


