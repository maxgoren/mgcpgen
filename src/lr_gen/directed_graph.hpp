#ifndef directed_graph_hpp
#define directed_graph_hpp
#include <iostream>
#include <unordered_map>
#include <unordered_set>
using namespace std;

using Symbol = int;

struct Transition {
    int dest;
    Symbol edgeLabel;
    Transition(int d, Symbol x) : dest(d), edgeLabel(x) { }
    bool operator<(const Transition& ot) const {
        return dest < ot.dest;
    }
    bool operator==(const Transition& ot) const {
        return dest == ot.dest && edgeLabel == ot.edgeLabel;
    }
    bool operator!=(const Transition& ot) const {
        return !(*this==ot);
    }
};

namespace std {
    template <> struct hash<Transition> {
        size_t operator()(const Transition& ot) const {
            unsigned long h = 17;
            h = h * 31 + ot.dest;
            h = h * 31 + ot.edgeLabel;
            return h;
        }
    };
}

class DirectedGraph {
    private:
        unordered_map<int, unordered_set<Transition>> adjlist;
        int edgecount;
    public:
        DirectedGraph() {
            edgecount = 0;
        }
        DirectedGraph(const DirectedGraph& dg) {
            edgecount = 0;
            for (auto const& m : dg.adjlist) {
                for (auto const& it : m.second)
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
            return adjlist[s].find(Transition(t, X)) != adjlist[s].end();
        }
        void addEdge(int s, int t, Symbol X) {
            if (!hasEdge(s, t, X)) {
                adjlist[s].insert(Transition(t, X));
                edgecount++;
            }
        }
        unordered_set<Transition>& adj(int v) {
            return adjlist[v];
        }
        void print() {
            for (auto const& e : adjlist) {
                cout<<e.first<<": \n";
                for (auto const& it : e.second) {
                    cout<<"\t "<<e.first<<" -("<<it.edgeLabel<<")-> "<<it.dest<<endl;
                }
            }
        }
        DirectedGraph& operator=(const DirectedGraph& dg) {
            if (this != &dg) {
                edgecount = 0;
                adjlist.clear();
                for (auto const & m : dg.adjlist)
                    for (auto const & it : m.second)
                        addEdge(m.first, it.dest, it.edgeLabel);
            }
            return *this;
        }
};

#endif