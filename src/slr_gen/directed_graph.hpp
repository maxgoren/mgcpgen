#ifndef directed_graph_hpp
#define directed_graph_hpp
#include <iostream>
#include <map>
using namespace std;

using Symbol = std::string;

struct Transition {
    int dest;
    Symbol edgeLabel;
    Transition* next;
    Transition(int d, Symbol x, Transition* t) : dest(d), edgeLabel(x), next(t) { }
};

class DirectedGraph {
    private:
        map<int, Transition*> adjlist;
        int edgecount;
    public:
        DirectedGraph() {
            edgecount = 0;
        }
        int V() {
            return adjlist.size();
        }
        int E() {
            return edgecount;
        }
        void addEdge(int s, int t, Symbol X) {
            adjlist[s] = new Transition(t, X, adjlist[s]);
            edgecount++;
        }
        Transition* adj(int v) {
            return adjlist[v];
        }
        void print() {
            for (auto e : adjlist) {
                for (auto it = e.second; it != nullptr; it = it->next) {
                    cout<<e.first<<" -("<<it->edgeLabel<<")-> "<<it->dest<<endl;
                }
            }
        }
};

#endif