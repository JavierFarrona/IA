/**
* Universidad de La Laguna
* Escuela Superior de Ingeniería y Tecnología
* Grado en Ingeniería Informática
* Asignatura: Inteligencia Artificial
* Curso: 3º
* C:\Users\javie\Desktop\Práctica\IA\Pr1\src
* Autor: Javier Farrona Cabrera
* Correo: alu0101541983@ull.edu.es
* Fecha 23 Sep 2026
* Archivo: Mapa.cpp
* Referencias: 
*     Enunciado de la práctica
* Historial de revisiones
*     20 Sep 2026 - Creación (primera versión) del código
*     22 Sep 2026 - Completa implementación de la clase Mapa y pruebas iniciales
*     23 Sep 2026 - Documentacion del código
*/

#include "Mapa.h"

#include <fstream>
#include <stdexcept>

// Construye un mapa vacío con dimensiones nulas.
Mapa::Mapa() : filas(0), columnas(0), origen(0, 0), destino(0, 0) {}

// Construye un mapa rectangular inicializado a obstáculo por defecto.
Mapa::Mapa(int filas, int columnas)
    : filas(filas), columnas(columnas), nodos(filas), origen(0, 0),
      destino(0, 0) {
    if (filas < 0 || columnas < 0) {
        throw std::invalid_argument("Las dimensiones del mapa no pueden ser negativas");
    }

    for (int fila = 0; fila < filas; ++fila) {
        nodos[fila].reserve(columnas);
        for (int columna = 0; columna < columnas; ++columna) {
            nodos[fila].emplace_back(fila, columna, -1);
        }
    }
}

// Carga el mapa desde un fichero de texto donde cada valor representa el coste
// de la celda y el origen y destino se identifican con 0 y 10.
void Mapa::cargarDesdeFichero(const std::string& nombreFichero) {
    std::ifstream fichero(nombreFichero);
    if (!fichero) {
        throw std::runtime_error("No se pudo abrir el fichero: " + nombreFichero);
    }

    std::vector<std::vector<int>> valores;
    std::vector<int> fila;
    int valor;

    while (fichero >> valor) {
        fila.push_back(valor);
        if (fichero.peek() == '\n' || fichero.peek() == '\r') {
            valores.push_back(fila);
            fila.clear();
        }
    }

    if (!fila.empty()) {
        valores.push_back(fila);
    }

    if (valores.empty() || valores.front().empty()) {
        throw std::runtime_error("El mapa esta vacio");
    }

    columnas = static_cast<int>(valores.front().size());
    filas = static_cast<int>(valores.size());

    for (const auto& filaLeida : valores) {
        if (static_cast<int>(filaLeida.size()) != columnas) {
            throw std::runtime_error("El mapa no es rectangular");
        }
    }

    nodos.assign(filas, std::vector<Nodo>());
    bool tieneOrigen = false;
    bool tieneDestino = false;

    for (int filaIndice = 0; filaIndice < filas; ++filaIndice) {
        for (int columna = 0; columna < columnas; ++columna) {
            const int coste = valores[filaIndice][columna];
            nodos[filaIndice].emplace_back(filaIndice, columna, coste);

            if (coste == 0) {
                origen = {filaIndice, columna};
                tieneOrigen = true;
            } else if (coste == 10) {
                destino = {filaIndice, columna};
                tieneDestino = true;
            }
        }
    }

    if (!tieneOrigen || !tieneDestino) {
        throw std::runtime_error("El mapa debe contener un origen (0) y un destino (10)");
    }
}

// Guarda el mapa actual en disco, sustituyendo la ruta encontrada por asteriscos.
void Mapa::guardarConCamino(const std::string& nombreFichero) const {
    std::ofstream fichero(nombreFichero);
    if (!fichero) {
        throw std::runtime_error("No se pudo crear el fichero: " + nombreFichero);
    }

    for (int fila = 0; fila < filas; ++fila) {
        for (int columna = 0; columna < columnas; ++columna) {
            if (columna > 0) {
                fichero << ' ';
            }

            const Nodo& nodo = nodos[fila][columna];
            if (nodo.perteneceAlCamino()) {
                fichero << '*';
            } else {
                fichero << nodo.getCoste();
            }
        }
        fichero << '\n';
    }
}

// Comprueba si una posición está dentro de los límites del mapa.
bool Mapa::esValida(const Posicion& posicion) const {
    return posicion.first >= 0 && posicion.first < filas &&
           posicion.second >= 0 && posicion.second < columnas;
}

// Comprueba si la posición es válida y no corresponde a un obstáculo.
bool Mapa::esTransitable(const Posicion& posicion) const {
    return esValida(posicion) && nodos[posicion.first][posicion.second].esTransitable();
}

// Genera los cuatro vecinos transitables de una celda en dirección arriba,
// derecha, abajo e izquierda.
std::vector<Posicion> Mapa::obtenerVecinos(const Posicion& posicion) const {
    static const int desplazamientos[][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
    std::vector<Posicion> vecinos;

    for (const auto& desplazamiento : desplazamientos) {
        Posicion vecino(posicion.first + desplazamiento[0],
                        posicion.second + desplazamiento[1]);
        if (esTransitable(vecino)) {
            vecinos.push_back(vecino);
        }
    }

    return vecinos;
}

// Devuelve la referencia constante a un nodo existente dentro del mapa.
const Nodo& Mapa::obtenerNodo(const Posicion& posicion) const {
    if (!esValida(posicion)) {
        throw std::out_of_range("Posicion fuera del mapa");
    }
    return nodos[posicion.first][posicion.second];
}

// Devuelve la referencia modificable a un nodo existente dentro del mapa.
Nodo& Mapa::obtenerNodo(const Posicion& posicion) {
    if (!esValida(posicion)) {
        throw std::out_of_range("Posicion fuera del mapa");
    }
    return nodos[posicion.first][posicion.second];
}

int Mapa::getFilas() const { return filas; }
int Mapa::getColumnas() const { return columnas; }
Posicion Mapa::getOrigen() const { return origen; }
Posicion Mapa::getDestino() const { return destino; }

// Actualiza el estado visual del mapa para reflejar la solución encontrada.
void Mapa::marcarCamino(const std::vector<Posicion>& camino) {
    for (auto& fila : nodos) {
        for (Nodo& nodo : fila) {
            nodo.marcarCamino(false);
        }
    }

    for (const Posicion& posicion : camino) {
        if (esValida(posicion)) {
            nodos[posicion.first][posicion.second].marcarCamino(true);
        }
    }
}