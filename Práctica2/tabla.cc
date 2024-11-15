#include "tabla.h"

// Class IterationInfo
// Parametrized constructor
IterationInfo::IterationInfo(const Node& current, const std::vector<Node>& generatedNodes, const std::vector<Node>& inspectedNodes, int accumulatedCost) {
    current_ = current;
    generatedNodes_ = generatedNodes;
    inspectedNodes_ = inspectedNodes;
    accumulatedCost_ = accumulatedCost;
}

// Getters
Node IterationInfo::getCurrent() const {
    return current_;
}

std::vector<Node> IterationInfo::getGeneratedNodes() const {
    return generatedNodes_;
}

std::vector<Node> IterationInfo::getInspectedNodes() const {
    return inspectedNodes_;
}

int IterationInfo::getAccumulatedCost() const {
    return accumulatedCost_;
}


// Class IterationLogger
// Log an iteration
void IterationLogger::logIteration(const Node& currentNode, const std::vector<Node>& generatedNodes, const std::vector<Node>& inspectedNodes, int accumulatedCost) {
    iterations_.emplace_back(currentNode, generatedNodes, inspectedNodes, accumulatedCost);
}

// Save the iterations to a file
void IterationLogger::saveToFile(std::ofstream& outFile, int rows, int cols, Node& start, Node& goal) const {
    outFile << "Instancia\tFilas\tColumnas\tNodo-de-Entrada\tNodo-de-Salida\tCamino\tCoste\tNodos-Generados\t  Nodos-Inspeccionados\n";
    char letra = 'a';
    for (size_t i = 0; i < iterations_.size(); ++i) {
        const auto& iteration = iterations_[i];
        if(i < 9) outFile << i + 1 << std::setw(13);
        else outFile << i + 1 << std::setw(12);
        outFile << rows << std::setw(9)
            << cols << std::setw(10)
            << "(" << start.getX() + 1 << "," << start.getY() + 1 << ")" << std::setw(12)
            << "(" << goal.getX() + 1 << "," << goal.getY() + 1 << ")" << std::setw(11);
        outFile << letra << std::setw(10);

        outFile << iteration.getAccumulatedCost() << std::setw(7)
            << iteration.getGeneratedNodes().size() << std::setw(19)
            << iteration.getInspectedNodes().size() << "\n";

        letra++;
        if(letra > 'z') letra = 'A';
    }
    outFile << std::endl << "Caminos:\n";
    letra = 'a';
    for (size_t i = 0; i < iterations_.size(); ++i) {
        const auto& iteration = iterations_[i];
        outFile << letra << std::setw(12);
        outFile << "[";
        for(size_t i = 0; i < iteration.getInspectedNodes().size(); ++i) {
            outFile << "(" << iteration.getInspectedNodes()[i].getX() + 1 << "," << iteration.getInspectedNodes()[i].getY() + 1 << ")";
            if(i != iteration.getInspectedNodes().size() - 1) {
                outFile << ", ";
            }
        }
        letra++;
        if(letra > 'z') letra = 'A';
        outFile << "]\n";
    }
    outFile.close();
}
