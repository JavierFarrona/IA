#ifndef NODO_H
#define NODO_H

#include <utility>

// Representa una posición como (fila, columna).
using Posicion = std::pair<int, int>;

class Nodo {
public:
    // Crea un nodo vacío con los valores iniciales de la búsqueda.
    Nodo();

    // Crea un nodo con su posición y el coste de entrada de la celda.
    Nodo(int fila, int columna, int coste);

    // Devuelve la posición del nodo como (fila, columna).
    Posicion getPosicion() const;

    // Devuelve la fila que ocupa el nodo.
    int getFila() const;

    // Devuelve la columna que ocupa el nodo.
    int getColumna() const;

    // Devuelve el coste de entrada en la celda.
    int getCoste() const;

    // Devuelve el coste acumulado desde el origen, g(n).
    int getCosteAcumulado() const;

    // Devuelve la estimación hasta el destino, h(n).
    int getCosteHeuristico() const;

    // Devuelve la función de evaluación f(n) = g(n) + h(n).
    int getCosteTotal() const;

    // Devuelve el nodo anterior en el camino actual.
    Nodo* getPadre() const;

    // Indica si el nodo representa un obstáculo.
    bool esObstaculo() const;

    // Indica si el nodo se puede utilizar para desplazarse.
    bool esTransitable() const;

    // Indica si el nodo pertenece al camino solución.
    bool perteneceAlCamino() const;

    // Actualiza los costes de A* y el padre del nodo.
    void actualizarCostes(int costeAcumulado, int costeHeuristico,
                          Nodo* padre);

    // Marca o desmarca el nodo como parte del camino solución.
    void marcarCamino(bool esCamino);

    // Restablece los datos temporales de búsqueda del nodo.
    void reiniciarBusqueda();

private:
    // Coordenadas del nodo dentro del mapa.
    Posicion posicion;

    // Coste de atravesar la celda; -1 identifica un obstáculo.
    int coste;

    // Indica si el nodo forma parte del camino encontrado.
    bool esCamino;

    // Coste acumulado desde el origen, g(n).
    int costeAcumulado;

    // Coste heurístico estimado hasta el destino, h(n).
    int costeHeuristico;

    // Nodo anterior utilizado para reconstruir el camino.
    Nodo* padre;
};

#endif