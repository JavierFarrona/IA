/**
* Universidad de La Laguna
* Escuela Superior de Ingeniería y Tecnología
* Grado en Ingeniería Informática
* Asignatura: Inteligencia Artificial
* Curso: 3ª
* C:\Users\Usuario\Documents\GitHub\ULL-2.1\IA\Práctica2
* Autor: Javier Farrona Cabrera
* Correo: alu0101541983@ull.edu.es
* Fecha 07 Oct 2024
* Archivo: grafo.cpp
* Referencias: 
*     Enunciado de la práctica
* Historial de revisiones
*     07 Oct 2024 - Creación (primera versión) del código
*/

#include "grafo.h"  // Incluye la definición de la clase Grafo
#include <queue>    // Para utilizar la cola de prioridad (priority_queue)
#include <cmath>    // Para funciones matemáticas (si es necesario)
#include <limits>   // Para establecer límites numéricos
#include <iostream> // Para la entrada/salida estándar

using namespace std;

// Método para cargar el grafo desde un archivo
void Grafo::loadGrafo(string nombre_fichero) {
    ifstream archivo(nombre_fichero);  // Abre el archivo de entrada
    if (!archivo.is_open()) {           // Verifica si el archivo se abre correctamente
        cout << "Error al abrir el archivo: " << nombre_fichero << endl;
        return;
    }

    // Lee la anchura y altura del laberinto
    archivo >> anchura_ >> altura_;

    // Redimensiona la matriz para el laberinto
    matriz_.resize(anchura_, vector<int>(altura_));

    // Lee el laberinto y detecta las coordenadas de inicio (3) y fin (4)
    for (int i = 0; i < anchura_; i++) {
        for (int j = 0; j < altura_; j++) {
            archivo >> matriz_[i][j];  // Lee cada celda del laberinto
            if (matriz_[i][j] == 3) {
                inicio_ = {i, j};  // Detecta la entrada
            } else if (matriz_[i][j] == 4) {
                fin_ = {i, j};     // Detecta la salida
            }
        }
    }

    archivo.close();  // Cierra el archivo
}


// Método para imprimir el laberinto en la consola
void Grafo::imprimir_laberinto() {
    for (int i = 0; i < anchura_; i++) {
        for (int j = 0; j < altura_; j++) {
            // Imprime un símbolo basado en el valor de la celda
            if (matriz_[i][j] == 1) {
                cout << "🧱";  // Pared
            } else if (matriz_[i][j] == 0) {
                cout << "⬜";  // Camino
            } else if (matriz_[i][j] == 3) {
                cout << "🚪";  // Entrada
            } else if (matriz_[i][j] == 4) {
                cout << "🏁";  // Salida
            } else if (matriz_[i][j] == 2) {
                cout << "🚶";  // Camino recorrido
            } 
        }
        cout << endl;  // Nueva línea al final de cada fila
    }
}


// Método para cambiar las coordenadas de inicio
void Grafo::CambiarInicio() {
    int x, y;
    cout << "Introduce las coordenadas x e y del nuevo inicio: ";
    cin >> x >> y;  // Lee las nuevas coordenadas
    // Verifica que las coordenadas sean válidas
    if (x < 1 || x > anchura_ || y < 1 || y > altura_) {
        cout << "Coordenadas inválidas." << endl;
        return;
    }
    matriz_[inicio_.first][inicio_.second] = 1;  // Marca la antigua posición como pared
    inicio_ = {x, y};                             // Actualiza la nueva posición de inicio
    matriz_[x - 1][y - 1] = 3;                    // Marca la nueva posición como entrada
}


// Método para cambiar las coordenadas de fin
void Grafo::CambiarFin() {
    int x, y;
    cout << "Introduce las coordenadas x e y de la nueva salida: ";
    cin >> x >> y;  // Lee las nuevas coordenadas
    // Verifica que las coordenadas sean válidas
    if (x < 1 || x > anchura_ || y < 1 || y > altura_) {
        cout << "Coordenadas inválidas." << endl;
        return;
    }
    matriz_[fin_.first][fin_.second] = 1;  // Marca la antigua posición como pared
    fin_ = {x - 1, y - 1};                  // Actualiza la nueva posición de fin
    matriz_[x - 1][y - 1] = 4;              // Marca la nueva posición como salida
}


// Método para imprimir el laberinto y el camino encontrado en un archivo
void Grafo::ImprimirFicheroResultados(string nombre_fichero) {
    ofstream archivo(nombre_fichero);  // Abre el archivo de salida
    if (!archivo.is_open()) {           // Verifica si se abre correctamente
        cout << "Error al abrir el archivo: " << nombre_fichero << endl;
        return;
    }

    // Imprime el laberinto en el archivo
    for (int i = 0; i < anchura_; i++) {
        for (int j = 0; j < altura_; j++) {
            // Imprime un símbolo basado en el valor de la celda
            if (matriz_[i][j] == 1) {
                archivo << "🧱";  // Pared
            } else if (matriz_[i][j] == 0) {
                archivo << "⬜";  // Camino
            } else if (matriz_[i][j] == 3) {
                archivo << "🚪";  // Entrada
            } else if (matriz_[i][j] == 4) {
                archivo << "🏁";  // Salida
            } else if (matriz_[i][j] == 2) {
                archivo << "🚶";  // Camino recorrido
            } 
        }
        archivo << endl;  // Nueva línea al final de cada fila
    }

    archivo.close();  // Cierra el archivo
}


// Método para obtener vecinos de un nodo en la matriz
vector<pair<int, int>> Grafo::obtenerVecinos(int x, int y) {
    vector<pair<int, int>> vecinos;  // Vector para almacenar las coordenadas de los vecinos
    vector<pair<int, int>> movimientos = {
        {-1, 0}, {1, 0}, {0, -1}, {0, 1},    // Movimientos ortogonales (arriba, abajo, izquierda, derecha)
        {-1, -1}, {-1, 1}, {1, -1}, {1, 1}   // Movimientos diagonales
    };

    for (auto [dx, dy] : movimientos) {
        int nx = x + dx;  // Nueva coordenada x
        int ny = y + dy;  // Nueva coordenada y

        // Verifica que las coordenadas estén dentro del rango y que no sea una pared (valor 1)
        if (nx >= 0 && nx < anchura_ && ny >= 0 && ny < altura_ && matriz_[nx][ny] != 1) {
            vecinos.push_back({nx, ny});  // Agrega la coordenada válida a los vecinos
        }
    }

    return vecinos;  // Devuelve el vector de vecinos
}


// Método para marcar el camino encontrado en la matriz
void Grafo::marcarCamino(Nodo* nodo, Grafo& grafo) {
    while (nodo != nullptr) {
        grafo.matriz_[nodo->x][nodo->y] = 2;  // Marca con 2
        nodo = nodo->padre;  // Va al nodo padre
    }
}


/**
 * @brief Realiza la búsqueda del camino más corto desde un punto de inicio hasta un punto objetivo utilizando el algoritmo A*.
 * 
 * @param inicio Coordenadas del punto de inicio (par de enteros).
 * @param objetivo Coordenadas del punto objetivo (par de enteros).
 * 
 * Esta función implementa el algoritmo A* para encontrar el camino más corto en un grafo representado como un laberinto.
 * Utiliza una cola de prioridad para explorar primero los nodos más prometedores, combinando el coste acumulado y 
 * una heurística que estima el coste restante hasta el objetivo.
 */
void Grafo::aStar(Coordenadas inicio, Coordenadas objetivo, const std::string& NombreArchivo) {
    std::ofstream out(NombreArchivo);
    if (!nombreArchivo.is_open()) {
        std::cerr << "Error al abrir el archivo de salida." << std::endl;
        return;
    }
    std::vector<Node> openList;
    std::vector<Node> closedList;
    std::vector<Node> generatedNodes;

    start.setCost(0);
    start.heuristica(inicio.first, inicio.second, objetivo.first, objetivo.second);
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



/**
 * @brief Realiza la búsqueda del camino más corto utilizando una versión modificada del algoritmo A*.
 * 
 * @param inicio Coordenadas del punto de inicio (par de enteros).
 * @param objetivo Coordenadas del punto objetivo (par de enteros).
 * 
 * Esta versión del algoritmo A* utiliza una heurística modificada para encontrar el camino más corto en un laberinto.
 */
void Grafo::aStarModificado(Coordenadas inicio, Coordenadas objetivo, const std::string& NombreArchivo) {
    std::ofstream nombreArchivo(NombreArchivo);
    if (!nombreArchivo.is_open()) {
        std::cerr << "Error al abrir el archivo de salida." << std::endl;
        return;
    }
    
    std::priority_queue<Nodo> frontera;
    frontera.push(Nodo(inicio.first, inicio.second, 0, heuristicaModificada(inicio.first, inicio.second, objetivo.first, objetivo.second)));

    std::vector<std::vector<float>> coste(altura_, std::vector<float>(anchura_, std::numeric_limits<float>::infinity()));
    coste[inicio.first][inicio.second] = 0;

    Nodo* nodoFinal = nullptr; // Inicializar puntero para el nodo final
    int iteracion = 0;  // Contador de iteraciones

    nombreArchivo << "Buscando camino..." << std::endl;
    nombreArchivo << "Inicio: (" << inicio.first << ", " << inicio.second << ")" << std::endl;
    nombreArchivo << "Objetivo: (" << objetivo.first << ", " << objetivo.second << ")" << std::endl;
    nombreArchivo << "--------------------------------" << std::endl;
    nombreArchivo << "Iteración 0" << std::endl;
    nombreArchivo << "Nodos generados: (" << inicio.first << ", " << inicio.second << ") ";
    nodosGenerados.push_back({inicio.first, inicio.second});
    nombreArchivo << "\nNodos inspeccionados: -" << std::endl;
    iteracion++;

    // Bucle principal del algoritmo A* modificado
    while (!frontera.empty()) {
        Nodo actual = frontera.top(); // Obtener el nodo con menor coste
        frontera.pop();
        nombreArchivo << "--------------------------------" << std::endl;
        nombreArchivo << "Iteración " << iteracion << std::endl;

        nodosInspeccionados.push_back({actual.x, actual.y});
        nombreArchivo << "Nodos inspeccionados: (" << actual.x << ", " << actual.y << ")" << std::endl;

        // Comprobar si hemos llegado al objetivo
        if (actual.x == objetivo.first && actual.y == objetivo.second) {
            nodoFinal = new Nodo(actual);  // Guardar el nodo final
            break; // Salir del bucle si se encuentra el objetivo
        }

        // Obtener vecinos del nodo actual
        for (auto [nx, ny] : this->obtenerVecinos(actual.x, actual.y)) {
            float nuevoCoste = coste[actual.x][actual.y]; // Coste hasta el nodo actual
            // Ajustar el coste para movimientos diagonales u ortogonales
            if (abs(actual.x - nx) == 1 && abs(actual.y - ny) == 1) {
                nuevoCoste += 7;  // Coste de 7 para movimientos diagonales
            } else {
                nuevoCoste += 5;  // Coste de 5 para movimientos ortogonales
            }

            // Si el nuevo coste es menor que el coste registrado para el vecino, actualizar
            if (nuevoCoste < coste[nx][ny]) {
                coste[nx][ny] = nuevoCoste; // Actualizar coste
                frontera.push(Nodo(nx, ny, nuevoCoste, heuristicaModificada(nx, ny, objetivo.first, objetivo.second), new Nodo(actual)));
                
                // Agregar nodo generado
                nodosGenerados.push_back({nx, ny});
            }
        }

        // Escribir en el archivo la lista de nodos generados en esta iteración
        nombreArchivo << "Nodos generados: ";
        for (const auto& nodo : nodosGenerados) {
            nombreArchivo << "(" << nodo.first << ", " << nodo.second << "), ";
        }
        nombreArchivo << std::endl;
        iteracion++;
    }

    // Escribir la lista final de nodos generados
    nombreArchivo << "Nodos generados: ";
    for (const auto& nodo : nodosGenerados) {
        nombreArchivo << "(" << nodo.first << ", " << nodo.second << "), ";
    }

    // Si se ha encontrado un camino, procesarlo
    if (nodoFinal != nullptr) {
        nombreArchivo << "\nCamino encontrado!" << std::endl;

        // Imprimir el coste total del camino encontrado
        float costeTotal = coste[nodoFinal->x][nodoFinal->y];
        nombreArchivo << "Coste total del camino: " << costeTotal << std::endl;
        costeTotal_ = costeTotal;

        // Marcar el camino en el laberinto
        Nodo* nodoActual2 = nodoFinal;
        while (nodoActual2 != nullptr) {
            this->matriz_[nodoActual2->x][nodoActual2->y] = 2;  // Marcar el camino con 2
            nodoActual2 = nodoActual2->padre;  // Ir al nodo padre
        }

        // Escribir el laberinto con el camino en el archivo
        for (int i = 0; i < anchura_; i++) {
            for (int j = 0; j < altura_; j++) {
                if (matriz_[i][j] == 1) {
                    nombreArchivo << "🧱";  // Pared
                } else if (matriz_[i][j] == 0) {
                    nombreArchivo << "⬜";  // Camino
                } else if (matriz_[i][j] == 3) {
                    nombreArchivo << "🚪";  // Entrada
                } else if (matriz_[i][j] == 4) {
                    nombreArchivo << "🏁";  // Salida
                } else if (matriz_[i][j] == 2) {
                    nombreArchivo << "🚶";  // Camino recorrido
                } 
            }
            nombreArchivo << endl;  // Nueva línea al final de cada fila
        }
        nombreArchivo << endl;  // Nueva línea al final de cada fila
        nombreArchivo << "---------------------------------\n";
        nombreArchivo << "Camino: ";
        Nodo* camino = nodoFinal;
        // Mostrar el camino recorrido en el archivo
        while (camino) {
            nombreArchivo << "(" << camino->x << ", " << camino->y << ")";
            if (camino->padre) nombreArchivo << " -> ";
            camino = camino->padre;
        }
    } else {
        nombreArchivo << "\nNo se encontró camino." << std::endl; // Mensaje si no se encuentra camino
    }
}


/**
 * @brief Guarda el grafo en un archivo especificado.
 *
 * Esta función toma un objeto de tipo Grafo y lo guarda en un archivo de texto
 * cuyo nombre se especifica en el parámetro nombreArchivo. El grafo se guarda
 * en formato de matriz, donde cada fila de la matriz se escribe en una nueva
 * línea del archivo.
 *
 * @param grafo El objeto Grafo que se desea guardar.
 * @param nombreArchivo El nombre del archivo donde se guardará el grafo.
 */

void Grafo::guardarGrafo(const Grafo& grafo, const std::string& nombreArchivo) {
    std::ofstream archivoSalida(nombreArchivo);
    if (!archivoSalida.is_open()) {
        std::cerr << "Error al abrir el archivo de salida." << std::endl;
        return;
    }
    for (int i = 0; i < grafo.altura_; ++i) {
        for (int j = 0; j < grafo.anchura_; ++j) {
            archivoSalida << grafo.matriz_[i][j] << " ";
        }
        archivoSalida << std::endl;
    }
    archivoSalida.close();
}
