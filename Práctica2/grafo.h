/**
* Universidad de La Laguna
* Escuela Superior de Ingeniería y Tecnología
* Grado en Ingeniería Informática
* Asignatura: Computabilidad y Algoritmia
* Curso: 2º
* C:\Users\Usuario\Documents\GitHub\ULL-2.1\IA\Práctica2
* Autor: Javier Farrona Cabrera
* Correo: alu0101541983@ull.edu.es
* Fecha 07 Oct 2024
* Archivo: grafo.h
* Referencias: 
*     Enunciado de la práctica
* Historial de revisiones
*     07 Oct 2024 - Creación (primera versión) del código
*/

#ifndef Grafo_H
#define Grafo_H

#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>

#include "node.h"

class Grafo {
 public:
  // Parametrized constructor
  Grafo(std::ifstream& filename);

  // Getters
  int getRows() const;
  int getCols() const;
  int getGrafo(int x, int y) const;
  Node getStart() const;
  Node getGoal() const;

  // Setters
  void setStart(int x, int y);
  void setGoal(int x, int y);

  // E/S
  void printGrafo(std::ostream& out);
  void printGrafo(const std::vector<Node>& path, std::ostream& out);
  void readGrafo(std::ifstream& file);

  // Heuristic function
  void aStar(Node start, Node goal, std::vector<Node>& path, std::ofstream& output, IterationLogger& logger);

  // Functions
  int CalculateCost(std::vector<Node>& path);
  void changeCoordinatesStart(int x, int y, Node& start);
  void changeCoordinatesGoal(int x, int y, Node& goal);

 private:
  int rows_;
  int cols_;
  std::vector<std::vector<int>> matriz_;
  Node start_;
  Node goal_;
};

#endif // Grafo_H