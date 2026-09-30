#ifndef serialize_hpp
#define serialize_hpp
#include <fstream>
#include "../cfg/cfg.hpp"
#include "directed_graph.hpp"
using namespace std;

using GoToTable = map<int,map<Symbol,string>>;
using ActionTable = map<int,map<Symbol,string>>;

class FileWriter {
    private:
        void printPrelude(ostream& ofile);
        void printProductions(ostream& os, Grammar& G, string name);
        template <class Iterable>
        void printTables(ostream& os, int nt, Iterable table, string tableName);
        template <class Iterable>
        void printRow(string tableName, int rowNum, ostream& os, Iterable& row);
        void printTable(string tableName, ostream& os, vector<int>& realrows);
        void printActionRegistrar(ostream& os, Grammar& G);
    public:
        FileWriter();
        void write(ostream& os, Grammar& G, DirectedGraph& cfsm, GoToTable& goTab, ActionTable& actTable);
};

#endif