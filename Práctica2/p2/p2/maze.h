#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <fstream>
#include <algorithm>
#include <iterator>

#include "node.h"
#include "data_table.h"

class Maze {
 public:
  // Parametrized constructor
  Maze(std::ifstream& filename);

  // Getters
  int getRows() const;
  int getCols() const;
  int getMaze(int x, int y) const;
  Node getStart() const;
  Node getGoal() const;

  // Setters
  void setStart(int x, int y);
  void setGoal(int x, int y);

  // E/S
  void printMaze(std::ostream& out);
  void printMaze(const std::vector<Node>& path, std::ostream& out);
  void readMaze(std::ifstream& file);

  // Heuristic function
  void aStar(Node start, Node goal, std::vector<Node>& path, std::ofstream& output, IterationLogger& logger);

  // Functions
  int CalculateCost(std::vector<Node>& path);
  void changeCoordinatesStart(int x, int y, Node& start);
  void changeCoordinatesGoal(int x, int y, Node& goal);

 private:
  int rows_;
  int cols_;
  std::vector<std::vector<int>> maze_;
  Node start_;
  Node goal_;
};

#endif // MAZE_H