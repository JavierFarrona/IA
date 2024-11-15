#include "maze.h"

// Parametrized constructor
Maze::Maze(std::ifstream& file) {
    readMaze(file);
}

// Getters
int Maze::getRows() const {
    return rows_;
}

int Maze::getCols() const {
    return cols_;
}

int Maze::getMaze(int x, int y) const {
    return maze_[x][y];
}

Node Maze::getStart() const {
    return start_;
}

Node Maze::getGoal() const {
    return goal_;
}

// Setters
void Maze::setStart(int x, int y) {
    start_ = Node(x,y);
}

void Maze::setGoal(int x, int y) {
    goal_ = Node(x,y);
}

// E/S
void Maze::printMaze(std::ostream& out) {
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            if(maze_[i][j] == 1) out << "█";
            else if(maze_[i][j] == 0) out << "░";
            else if(maze_[i][j] == 3) out << "E";
            else out << "S";
        }
        out << std::endl;
    }
}

void Maze::printMaze(const std::vector<Node>& path, std::ostream& out) {
    // Create a copy of the maze to mark the path
    std::vector<std::vector<int>> mazeCopy = maze_;

    // Mark the path in the maze copy
    for (const auto& node : path) {
        mazeCopy[node.getX()][node.getY()] = 2; // Use 2 to represent the path
    }

    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            if (mazeCopy[i][j] == 1) out << "█";
            else if (mazeCopy[i][j] == 0) out << "░";
            else if (mazeCopy[i][j] == 3) out << "E";
            else if (mazeCopy[i][j] == 2) out << "*"; // Path
            else out << "S";
        }
        out << std::endl;
    } 
} 

void Maze::readMaze(std::ifstream& file) {
    file >> rows_ >> cols_;
    maze_.resize(rows_, std::vector<int>(cols_));
    for (int i = 0; i < rows_; i++) {
        for (int j = 0; j < cols_; j++) {
            file >> maze_[i][j];
            if(maze_[i][j] == 3) {
                setStart(i,j);
            } else if (maze_[i][j] == 4) {
                setGoal(i,j);
            }
        }
    }
    file.close();
}

// Dentro de la función aStar
void Maze::aStar(Node start, Node goal, std::vector<Node>& path, std::ofstream& out, IterationLogger& logger) {
    std::vector<Node> openList;
    std::vector<Node> closedList;
    std::vector<Node> generatedNodes;

    start.setCost(0);
    start.calculateHeuristic(goal.getX(), goal.getY(), start.getX(), start.getY());
    // start.euclideanDistance(goal.getX(), goal.getY(), start.getX(), start.getY());
    openList.push_back(start);

    int iterations = 1;
    int accumulatedCost = 0;

    // std::srand(static_cast<unsigned>(std::time(nullptr)));

    while (!openList.empty()) {
        // Find the node with the lowest f value
        auto currentIt = std::min_element(openList.begin(), openList.end(), [](const Node& a, const Node& b) {
            return (a.getCost() + a.getHeuristic()) < (b.getCost() + b.getHeuristic());
        });
        int currentIndex = std::distance(openList.begin(), currentIt);

        // std::sort(openList.begin(), openList.end(), [](const Node& a, const Node& b) {
        //     return (a.getCost() + a.getHeuristic()) < (b.getCost() + b.getHeuristic());
        // });

        // int numElements = std::min(3, static_cast<int>(openList.size()));
        // std::vector<Node> smallestNodes(openList.begin(), openList.begin() + numElements);

        // int currentIndex = std::rand() % numElements;

        Node current = openList[currentIndex];

        // Mostrar la iteración
        out << "Iteración: " << iterations++ << "\n";
        // Mostrar nodo actual
        out << "Nodo actual: (" << current.getX() + 1 << "," << current.getY() + 1 << ")\n";

        if (current.getX() == goal.getX() && current.getY() == goal.getY()) {
            // Nodos generados
            out << "Nodos generados: -\n";
            out << "Nodos inspeccionados: " << current.getX() + 1 << "," << current.getY() + 1<< "\n";
            // Reconstruct path
            while (!(current.getX() == start.getX() && current.getY() == start.getY())) {
                path.push_back(current);
                std::pair<int, int> parent = current.getParent();
                auto it = std::find_if(closedList.begin(), closedList.end(), [&parent](const Node& n) {
                    return n.getX() == parent.first && n.getY() == parent.second;
                });
                if (it != closedList.end()) {
                    current = *it;
                } else {
                    break;
                }
            }
            closedList.push_back(goal);
            path.push_back(start);
            accumulatedCost = CalculateCost(path);
            logger.logIteration(current, generatedNodes, closedList, accumulatedCost);
            std::reverse(path.begin(), path.end());
            return;
        }

        closedList.push_back(current);
        openList.erase(openList.begin() + currentIndex);

        // Define directions and their respective costs
        std::vector<std::pair<std::pair<int, int>, int>> directions = {
            {{-1, 0}, 5}, {{1, 0}, 5}, {{0, -1}, 5}, {{0, 1}, 5}, // Lateral and vertical
            {{-1, -1}, 7}, {{-1, 1}, 7}, {{1, -1}, 7}, {{1, 1}, 7} // Diagonal
        };

        for (const auto& dir : directions) {
            int newX = current.getX() + dir.first.first;
            int newY = current.getY() + dir.first.second;
            int moveCost = dir.second;

            if (newX < 0 || newY < 0 || newX >= rows_ || newY >= cols_ || maze_[newX][newY] == 1) {
                continue;
            }

            Node neighbor(newX, newY);
            neighbor.setParent({current.getX(), current.getY()});
            neighbor.setCost(current.getCost() + moveCost);
            neighbor.calculateHeuristic(goal.getX(), goal.getY(), newX, newY);
            // neighbor.euclideanDistance(goal.getX(), goal.getY(), newX, newY);

            if (std::find_if(closedList.begin(), closedList.end(), [&neighbor](const Node& n) {
                return n.getX() == neighbor.getX() && n.getY() == neighbor.getY();
            }) != closedList.end()) {
                continue;
            }

            auto it = std::find_if(openList.begin(), openList.end(), [&neighbor](const Node& n) {
                return n.getX() == neighbor.getX() && n.getY() == neighbor.getY();
            });

            if (it == openList.end() || neighbor.getCost() < it->getCost()) {
                openList.push_back(neighbor);
                generatedNodes.push_back(neighbor);
            }
        }

        // Mostrar nodos generados
        out << "Nodos generados: ";
        for (const auto& node : generatedNodes) {
            out << "(" << node.getX() + 1 << "," << node.getY() + 1 << ") ";
        }
        out << "\n";

        // Mostrar nodos inspeccionados
        out << "Nodos inspeccionados: ";
        for (const auto& node : closedList) {
            out << "(" << node.getX() + 1 << "," << node.getY() + 1<< ") ";
        }
        out << "\n" << std::endl;

        // Calcular costo en iteración
        accumulatedCost = current.getCost();

        logger.logIteration(current, generatedNodes, closedList, accumulatedCost);
    }
}

// Functions
int Maze::CalculateCost(std::vector<Node>& path) {
    int totalCost = 0;
    for (size_t i = 1; i < path.size(); ++i) {
        int dx = std::abs(path[i].getX() - path[i - 1].getX());
        int dy = std::abs(path[i].getY() - path[i - 1].getY());
        if (dx + dy == 1) {
            totalCost += 5; // Lateral move
        } else if (dx + dy == 2) {
            totalCost += 7; // Diagonal move
        }
    }
    return totalCost;
}

void Maze::changeCoordinatesStart(int x, int y, Node& start) {
    maze_[start.getX()][start.getY()] = 1;
    maze_[x][y] = 3;
}

void Maze::changeCoordinatesGoal(int x, int y, Node& goal) {
    maze_[goal.getX()][goal.getY()] = 1;
    maze_[x][y] = 4;
}