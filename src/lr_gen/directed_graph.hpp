#ifndef directed_graph_hpp
#define directed_graph_hpp
#include <iostream>
#include <map>
#include <list>
using namespace std;

using Symbol = std::string;

struct Transition {
    int dest;
    Symbol edgeLabel;
    Transition(int d, Symbol x) : dest(d), edgeLabel(x) { }
};

class DirectedGraph {
    private:
        map<int, list<Transition>> adjlist;
        int edgecount;
    public:
        DirectedGraph() {
            edgecount = 0;
        }
        DirectedGraph(const DirectedGraph& dg) {
            edgecount = 0;
            for (auto m : dg.adjlist) {
                for (auto it : m.second)
                    addEdge(m.first, it.dest, it.edgeLabel);
            }
        }
        ~DirectedGraph() {
    
        }
        int V() {
            return adjlist.size();
        }
        int E() {
            return edgecount;
        }
        bool hasEdge(int s, int t, Symbol X) {
            for (auto it : adjlist[s]) {
                if (it.dest == t && it.edgeLabel == X)
                    return true;
            }
            return false;
        }
        void addEdge(int s, int t, Symbol X) {
            adjlist[s].push_back(Transition(t, X));
            edgecount++;
        }
        list<Transition> adj(int v) {
            return adjlist[v];
        }
        void print() {
            for (auto e : adjlist) {
                for (auto it : e.second) {
                    cout<<e.first<<" -("<<it.edgeLabel<<")-> "<<it.dest<<endl;
                }
            }
        }
        DirectedGraph& operator=(const DirectedGraph& dg) {
            if (this != &dg) {
                edgecount = 0;
                for (auto m : dg.adjlist)
                    for (auto it : m.second)
                        addEdge(m.first, it.dest, it.edgeLabel);
            }
            return *this;
        }
};

#endif