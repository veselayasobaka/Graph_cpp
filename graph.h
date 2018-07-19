#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include <iomanip>
#include <vector>
#include <eigen3/Eigen/Dense>
using namespace std;
using namespace Eigen;

//Bound - keep two numbers - number of vortex, bounded with ours
//and the weigth of this bound
struct Bound
{
    int number_vortex;
    int weigth;
    Bound(int n, int w): number_vortex(n), weigth(w){}
    void show_bound() const
    {
        cout << '(' << number_vortex << ", " << weigth << ')';
    }
    ~Bound(){}
    friend bool operator ==(const Bound &lhs, const Bound &rhs);
};
//Vortex - structure to keep all data about some vortex:
//it's number, level, colour(mass), range of it's children and vector of bounds
struct Vortex
{
    int number = 0;
    int level = 0;
    int colour = 0;
    int child_range = 0;
    vector<Bound> bounds;
    Vortex(int n): number(n){}
    Vortex(int n, int l, int colour): number(n), level(l), colour(colour){}
    void add_bound(const Bound b)
    {
        bounds.push_back(b);
    }
    void show_vortex() const
    {
        cout << number << setw(4) << level << setw(4) << colour << setw(4) << child_range << ' ';
        for (auto it = bounds.begin();  it != bounds.end(); ++it)
        {
            it->show_bound();
        }
    }
    void move_bound_to_end(const int &b, const int &c)
    {
        Bound temp(b, c);
        auto it = find(bounds.begin(), bounds.end(), temp);
        (*it) = (*(bounds.end()-1));
        (*(bounds.end()-1)) = temp;
    }
};
//function for sort the Graaph
bool sortlevels(Vortex v1, Vortex v2);
bool waytosort(Vortex v1, Vortex v2);


// Graph - class for interaction with user.
//Graph keeps vector of vortexes and translates adjacency matrix to adjacency list(vector of vortexes).
//it's metods also cononize graph.

class Graph
{
private:
    vector<Vortex> vortexes;
public:
    Graph(MatrixXf m1);
    Graph(int q)
    {
        for (int i = 0; i < q; ++i)
        {
            Vortex v(i);
            vortexes.push_back(v);
        }
    }
    void show_graph() const
    {
        for (auto it = vortexes.begin();  it != vortexes.end(); ++it)
        {
            it->show_vortex();
            cout << endl;
        }
    }
    void set_colour(int n, int c)
    {
        vortexes[n].colour = c;
    }
    void set_colour()
    {
        for (auto it = vortexes.begin();  it != vortexes.end(); ++it)
        {
            cout << "Vortex " << it->number << ':';
            int a = 0;
            cin >> a;
            vortexes[it->number].colour = a;
        }
    }
    void set_bound(int n)
    {
        cout << "Vortex " << n << ':';
        int i = 0;
        int j = 0;
        cin >> i >> j;
        Bound b(i, j);
        vortexes[n].add_bound(b);
    }
    void set_bound()
    {
        for (auto it = vortexes.begin();  it != vortexes.end(); ++it)
        {
            cout << "Vortex " << it->number << ':';
            int i = 0;
            int j = 0;
            cin >> i >> j;
            Bound b(i, j);
            vortexes[it->number].add_bound(b);
        }
    }
    void set_levels();

};

#endif // GRAPH_H
