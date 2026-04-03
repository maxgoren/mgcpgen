#include <iostream>
#include <map>
#include <set>
#include "lexer_matrix.h"
using namespace std;

bool compareRows(int a, int b) {
    for (int i = 0; i < 256; i++) {
        if (matrix[a][i] != matrix[b][i])
            return false;
    }
    return (accept[a] != -1 && accept[b] != -1) || (accept[a] == -1 && accept[b] == -1);
}





int main() {
    int unique_sets = 42;
    map<int, set<int>> equals;
    for (int i = 0; i < 42; i++) {
        for (int j = 0; j < 42; j++) {
            if (i != j && compareRows(i, j)) {
                equals[i].insert(j);
            }
        }
    }
    for (int i = 0; i < 42; i++) {
        cout<<i<<": ";
        for (auto m : equals[i]) {
            cout<<m<<" ";
        }
        cout<<endl;
    }
}

