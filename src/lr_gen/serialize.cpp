#include "serialize.hpp"


FileWriter::FileWriter() {

}

void FileWriter::write(ostream& ofile, Grammar& G, DirectedGraph& cfsm, GoToTable& goTab, ActionTable& actTable) {
    printPrelude(ofile);
    printProductions(ofile, G, "prod");
    printTables(ofile, cfsm.V() , goTab, "goTab");
    printTables(ofile, cfsm.V(), actTable, "actTab");
    printActionRegistrar(ofile, G);
}

void FileWriter::printPrelude(ostream& ofile) {
    ofile<<"#include <vector>\n";
    ofile<<"#include <map>\n";
    ofile<<"#include <set>\n";
    ofile<<"#include <functional>\n";
    ofile<<"using namespace std; \n";
}
void FileWriter::printProductions(ostream& os, Grammar& G, string name) {
    os<<"enum NTSYMBOL {\n";
    os<<"\t DOLLARACCEPT,\n";
    int i = 0;
    for (auto t : G.nonterminals) {
        if (t != "#" && !t.empty()) {
            os<<t;
            if (i+1 < G.nonterminals.size())
                os<<", ";
            if (i > 1 && i % 5 == 0) 
                os<<endl;
        }
        i++;
    }
    os<<"\n};\n";
    os<<"struct Production {\n\t int id;\n\t int lhs;\n\t vector<int> rhs;\n\t string actsym;\n }; "<<endl;
    os<<"\nstatic const Production "<<name<<"[] = {\n \t {0, 0, {}, \"\"},\n"<<endl;
    int p = 0;
    for (auto e : G.prodById) {
        os<<"\t {"<<e.second.pid<<","<<e.second.lhs<<", ";
        os<<"{";
        for (int i = 0; i < e.second.rhs.size(); i++) {
            os<<e.second.rhs[i];
            if (i+1 < e.second.rhs.size())
                os<<", ";
        }
        os<<"}";
        os<<",\""<<e.second.action<<"\"}";
        if (p+1 < G.prodById.size()) {
            os<<", \n";
        } else os<<"\n";
        p++;
    }
    os<<"};\n";
}

template <class Iterable>
void FileWriter::printRow(string tableName, int rowNum, ostream& os, Iterable& row) {
    os<<"static const int "<<tableName<<"_row_"<<rowNum<<"[] = {";
    int i = 0;
    os<<row.size()<<",";
    for (auto entry : row) {
        if (isalpha(entry.second[0])) {
            if (entry.first == "$") {
                os<<"DOLLARACCEPT, ";
            } else {
                os<<entry.first<<", ";
            }
            switch (entry.second[0]) {
                case 's': os<<entry.second.substr(1); break;
                case 'r': os<<-stoi(entry.second.substr(1)); break;
                case 'a': os<<0; break;
            }
        } else {
            os<<entry.first<<", "<<entry.second;
        }
        if (i+1 < row.size())
            os<<", ";
        i++;
    }
    os<<"};\n";
}

void FileWriter::printTable(string tableName, ostream& os, vector<int>& realrows) {
    os<<"\nstatic const int *"<< tableName <<"[] = {\n";
    int i = 0;
    for (auto t : realrows) {
        if (t == -1) os<<"\t NULL";
        else os<<"\t "<<tableName<<"_row_"<<t;
        if (i+1 < realrows.size())
            os<<", ";
        i++;
        if (i > 4 && i % 5 == 0) os<<"\n";
    }
    os<<"\n};\n"<<endl;
}

template <class Iterable>
void FileWriter::printTables(ostream& os, int numStates, Iterable table, string tableName) {
    vector<int> realrows(numStates, -1);
    for (auto e : table) {
            int duppyRow = -1;
            for (auto q : table) {
                if (e.first != q.first && e.first > q.first && q.second == e.second) {
                    duppyRow = q.first; 
                    break;
                }
            }
        if (e.first >= realrows.size()) {
            realrows.push_back(e.first);
        }
        if (duppyRow < 0) {
            realrows[e.first] = e.first;
            printRow(tableName, e.first, os, e.second);
        } else {
            realrows[e.first] = duppyRow;
        }
    }
    printTable(tableName, os, realrows);
}

void FileWriter::printActionRegistrar(ostream& os, Grammar& G) {
    os<<"struct "<<G.returnType<<";\n";
    os<<"\nstatic const map<string, function<"<<G.returnType;
    os<<"*(vector<"<<G.returnType; 
    os<<"*>&)>> actions = {\n";
    int i = 0;
    for (auto actions : G.actionMap) {
        os<<"\t {\""<<actions.first<<"\","<<actions.second<<"}";
        if (i+1 < G.actionMap.size()) os<<", \n";
        else os<<"\n";
        i++;
    }
    os<<"\n};"<<endl;
}