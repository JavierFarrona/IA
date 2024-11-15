#include "code.h"

int main(int argc, char* argv[]) {
    if (!ComprobarParametros(argc, argv, 3)){ // Comprueba los parámetros de entrada
      return 1;
    }

    std::string entrada = argv[1];
    std::string salida = argv[2];
    std::ifstream input(entrada);
    std::ofstream output(salida);
    is_open(input, output);

    Maze maze(input);
    Node start = maze.getStart();
    Node goal = maze.getGoal();

    std::cout << "¿Quieres introducir un punto de inicio y un punto final? (s/n): ";
    char respuesta;
    std::cin >> respuesta;
    if (respuesta == 's') {
        std::cout << "Matriz-> Filas: " << maze.getRows() << " Columnas: " << maze.getCols() << std::endl;
        int x, y;
        std::cout << "Introduce las coordenadas del punto de inicio (x, y): ";
        std::cin >> x >> y;
        if(maze.getMaze(x - 1, y - 1) != 1) {
          std::cout << "No se puede introducir el punto de inicio en una no pared." << std::endl;
          return 1;
        }
        maze.changeCoordinatesStart(x - 1, y - 1, start);
        maze.setStart(x - 1, y - 1);
        std::cout << "Introduce las coordenadas del punto final (x, y): ";
        std::cin >> x >> y;
        if(maze.getMaze(x - 1, y - 1) != 1) {
            std::cout << "No se puede introducir el punto de salida en una no pared." << std::endl;
            return 1;
        }
        maze.changeCoordinatesGoal(x - 1, y - 1, goal);
        maze.setGoal(x - 1, y - 1);
        start = maze.getStart();
        goal = maze.getGoal();
    }

    output << "Maze: " << argv[1] << std::endl;
    maze.printMaze(output);

    std::vector<Node> path;
    IterationLogger logger;
    output << std::endl;
    maze.aStar(start, goal, path, output, logger);

    if (path.empty()) {
        output << "No path found." << std::endl;
        return 1;
    } else {
      output << "\nPath found:" << std::endl;
      maze.printMaze(path, output);
      output << "Camino: ";
      for(const auto& node : path) {
        output << "(" << node.getX() + 1 << "," << node.getY() + 1 << ") ";
      }

      int totalCost = maze.CalculateCost(path);
      output << "| Total Cost: " << totalCost << std::endl << std::endl;

      logger.saveToFile(output, maze.getRows(), maze.getCols(), start, goal);
    }

    return 0;
}