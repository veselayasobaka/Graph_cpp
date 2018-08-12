#include "graph.h"
using namespace std;
Graph readFile(const string &s);

int main(int argc, char* argv[])
{
    if (argc > 1)
    {
        Graph g1 = readFile(argv[1]);
        string a = g1.getCanonCode();
        cout << a << endl;
    }
    else
        cout << "Please enter the name of file containing graph" << endl;
    return 0;
}


