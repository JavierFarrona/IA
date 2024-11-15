/**
 * Universidad de La Laguna
 * Escuela Superior de Ingeniería y Tecnología
 * Grado en Ingeniería Informática
 * Asignatura: Inteligencia Artificial
 * Curso: 3º
 * C:\Users\Usuario\Desktop\ULL\IA\Práctica1
 * Autor: Javier Farrona Cabrera
 * Correo: alu0101541983@ull.edu.es
 * Fecha 24 Sep 2024
 * Archivo: main.cpp
 * Referencias:
 *     Enunciado de la práctica
 * Historial de revisiones
 *     24 Sep 2024 - Creación (primera versión) del código
 *     27 Sep 2024 - Lectrua de grafo desde archivo
 *     28 Sep 2024 - Implementación de mostrar matriz de forma provisional para
 * ver funcion de carga 29 Sep 2024 - Implementación de BFS y DFS 01 Oct 2024 -
 * Arreglo de bugs, errores y fallos
 */

#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "grafo.h"

using namespace std;

int main() {
  Grafo g(0);

  // Cargar el grafo desde un archivo
  string string;

  cin >> string;
  g.loadGraphFromFile(string);

  cout << "Matriz de adyacencia: " << endl;
  g.mostrarMatriz();

  cout << "Introduzca los nodos de inicio y fin: " << endl;

  int startVertex, endVertex;  // Nodo inicial (indexado desde 0)
  cin >> startVertex >> endVertex;

  cout << "Que busqueda desea realizar (BFS(1) o DFS(2) o (BFS_MODI(3)): "
       << endl;

  int search;
  cin >> search;

  ofstream output{"output.txt"};

  if (search == 1) {
    output << "\nBúsqueda en Anchura (BFS):\n";
    g.BFS(startVertex - 1, endVertex - 1, output);
  } else if (search == 2) {
    output << "\nBúsqueda en Profundidad (DFS):\n";
    g.DFS(startVertex - 1, endVertex - 1, output);
  } else if (search == 3) {
    // g.BFS_MODI(startVertex - 1, endVertex - 1, output);
  } else {
    output << "Opción no válida" << endl;
  }

  output.close();

  return 0;
}
