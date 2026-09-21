#ifndef MAPA_H
#define MAPA_H

#include "Nodo.h"

#include <string>
#include <vector>

class Mapa {
public:
    // Crea un mapa vacío.
    Mapa();

    // Crea un mapa con el número indicado de filas y columnas.
    Mapa(int filas, int columnas);

    // Lee las celdas, el origen y el destino desde un fichero.
    void cargarDesdeFichero(const std::string& nombreFichero);

    // Guarda el mapa en un fichero marcando el camino con asteriscos.
    void guardarConCamino(const std::string& nombreFichero) const;

    // Comprueba si una posición está dentro de los límites del mapa.
    bool esValida(const Posicion& posicion) const;

    // Comprueba si una posición está dentro del mapa y no es obstáculo.
    bool esTransitable(const Posicion& posicion) const;

    // Devuelve las posiciones vecinas transitables en las cuatro direcciones.
    std::vector<Posicion> obtenerVecinos(const Posicion& posicion) const;

    // Devuelve una referencia constante al nodo de una posición.
    const Nodo& obtenerNodo(const Posicion& posicion) const;

    // Devuelve una referencia modificable al nodo de una posición.
    Nodo& obtenerNodo(const Posicion& posicion);

    // Devuelve el número de filas del mapa.
    int getFilas() const;

    // Devuelve el número de columnas del mapa.
    int getColumnas() const;

    // Devuelve la posición del origen del robot.
    Posicion getOrigen() const;

    // Devuelve la posición del destino del robot.
    Posicion getDestino() const;

    // Marca como camino los nodos de la solución encontrada.
    void marcarCamino(const std::vector<Posicion>& camino);

private:
    // Número de filas del entorno.
    int filas;

    // Número de columnas del entorno.
    int columnas;

    // Matriz de nodos que representa todas las celdas del mapa.
    std::vector<std::vector<Nodo>> nodos;

    // Posición inicial del robot.
    Posicion origen;

    // Posición que debe alcanzar el robot.
    Posicion destino;
};

#endif