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
        DirectedGraph(const DirectedGraph& dg) {
            edgecount = 0;
            for (auto m : dg.adjlist) {
                for (auto it = m.second; it != nullptr; it = it->next)
                    addEdge(m.first, it->dest, it->edgeLabel);
            }
        }
        ~DirectedGraph() {
            for (auto m : adjlist) {
                auto it = m.second;
                while (it != nullptr) {
                    Transition* tmp = it;
                    it = it->next;
                    delete tmp;
                }
            }
        }
        int V() {
            return adjlist.size();
        }
        int E() {
            return edgecount;
        }
        bool hasEdge(int s, int t, Symbol X) {
            for (auto it = adjlist[s]; it != nullptr; it = it->next) {
                if (it->dest == t && it->edgeLabel == X)
                    return true;
            }
            return false;
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
        DirectedGraph& operator=(const DirectedGraph& dg) {
            if (this != &dg) {
                edgecount = 0;
                for (auto m : dg.adjlist)
                    for (auto it = m.second; it != nullptr; it = it->next)
                        addEdge(m.first, it->dest, it->edgeLabel);
            }
            return *this;
        }
};

#endif