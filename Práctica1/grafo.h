#ifndef GRAFO_H
#define GRAFO_H

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

class Grafo { 
  public:
    Grafo(int nodos) : nodos_(nodos) {
        matriz_.resize(nodos, vector<double>(nodos, 0));
    }

    void mostrarMatriz() {
        for (int i = 0; i < nodos_; ++i) {
            for (int j = 0; j < nodos_; ++j) {
                cout << matriz_[i][j] << "    ";
            }
            cout << endl;
        }
    }

    void loadGraphFromFile(const string& filename); 

    void BFS(int startVertex, int endVertex, ostream &out);

    void DFS(int startVertex, int endVertex, ostream &out);

    // void DFS_MenorCoste(int startVertex, int endVertex, ostream &out);
      

  private:
    int nodos_;
    vector<vector<double>> matriz_;
};

#endif // GRAFO_H