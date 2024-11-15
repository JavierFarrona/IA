#ifndef DATA_TABLE_H
#define DATA_TABLE_H

#include "node.h"
#include <fstream>
#include <iomanip>

class IterationInfo {
 public:
  // Parametrized constructor
  IterationInfo(const Node& current, const std::vector<Node>& generatedNodes, const std::vector<Node>& inspectedNodes, int accumulatedCost);

  // Getters
  Node getCurrent() const;
  std::vector<Node> getGeneratedNodes() const;
  std::vector<Node> getInspectedNodes() const;
  int getAccumulatedCost() const;

 private:
  Node current_;
  std::vector<Node> generatedNodes_;
  std::vector<Node> inspectedNodes_;
  int accumulatedCost_;
};

class IterationLogger {
 public:
    void logIteration(const Node& current, const std::vector<Node>& generatedNodes, const std::vector<Node>& inspectedNodes, int accumulatedCost);
    void saveToFile(std::ofstream& filename, int rows, int cols, Node& start, Node& goal) const;

 private:
  std::vector<IterationInfo> iterations_;
};

#endif // DATA_TABLE_H