#ifndef NODE_H
#define NODE_H

#include <iostream>
#include <vector>
#include <stdlib.h>
#include <math.h>

class Node {
 public:
  // Parametrized constructor
  Node();
  Node(int x, int y);
  Node(int x, int y, std::pair<int,int> parent, int cost, int heuristic);

  // Getters
  int getX() const;
  int getY() const;
  std::pair<int, int> getParent() const;
  int getCost() const;
  int getHeuristic() const;

  // Setters
  void setParent(std::pair<int, int> parent);
  void setCost(int cost);

  // Heuristic function
  void calculateHeuristic(int x_goal, int y_goal, int x, int y);
  void euclideanDistance(int x_goal, int y_goal, int x, int y);

 private:
  int x_;
  int y_;
  std::pair<int, int> parent_;
  int cost_;
  int heuristic_;
};

#endif // NODE_H