#include "nodo.h"

Nodo::Nodo() {
  x_ = 0;
  y_ = 0;
  parent_ = std::make_pair(-1, -1);
  cost_ = 0;
  heuristic_ = 0;
}

// Parametrized constructor
Nodo::Nodo(int x, int y, std::pair<int,int> parent, int cost, int heuristic) {
  x_ = x;
  y_ = y;
  parent_ = parent;
  cost_ = cost;
  heuristic_ = heuristic;
}

Nodo::Nodo(int x, int y) {
  x_ = x;
  y_ = y;
  parent_ = std::make_pair(-1, -1);
  cost_ = 0;
  heuristic_ = 0;
}

// Getters
int Nodo::getX() const {
  return x_;
}

int Nodo::getY() const {
  return y_;
}

std::pair<int, int> Nodo::getParent() const {
  return parent_;
}

int Nodo::getCost() const {
  return cost_;
}

int Nodo::getHeuristic() const {
  return heuristic_;
}

// Setters
void Nodo::setParent(std::pair<int, int> parent) {
  parent_ = parent;
}

void Nodo::setCost(int cost) {
  cost_ = cost;
}

// Heuristic function
void Nodo::calculateHeuristic(int x_goal, int y_goal, int x, int y) {
  heuristic_ = abs(x_goal - x) + abs(y_goal - y);
}

void Nodo::euclideanDistance(int x_goal, int y_goal, int x, int y) {
  heuristic_ = std::sqrt((x_goal - x) * (x_goal - x) + (y_goal - y) * (y_goal - y));
}