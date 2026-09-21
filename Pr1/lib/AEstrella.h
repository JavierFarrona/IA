#ifndef A_ESTRELLA_H
#define A_ESTRELLA_H

#include "Mapa.h"
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

struct ComparadorNodos {
    // Ordena los nodos de menor a mayor coste total f(n).
    bool operator()(const Nodo* izquierdo, const Nodo* derecho) const;
};

struct ResultadoBusqueda {
    // Secuencia de posiciones que forma el camino solución.
    std::vector<Posicion> camino;

    // Coste total del camino encontrado.
    int coste;

    // Posiciones generadas que siguen pendientes de inspección.
    std::set<Posicion> nodosGenerados;

    // Posiciones extraídas e inspeccionadas durante la búsqueda.
    std::set<Posicion> nodosInspeccionados;

    // Indica si se alcanzó el destino.
    bool encontrado;
};

class AEstrella {
public:
    // Crea una búsqueda A* asociada al mapa indicado.
    explicit AEstrella(Mapa& mapa);

    // Ejecuta A* y devuelve el camino y sus estadísticas.
    ResultadoBusqueda buscar();

    // Calcula la distancia Manhattan ponderada por el menor coste transitable.
    int calcularHeuristica(const Posicion& posicion) const;

private:
    // Reconstruye el camino siguiendo los padres desde el destino.
    std::vector<Posicion> reconstruirCamino(const Nodo* nodoFinal) const;

    // Mapa sobre el que se ejecuta la búsqueda.
    Mapa& mapa;

    // Cola de prioridad con los nodos pendientes de inspeccionar.
    std::priority_queue<Nodo*, std::vector<Nodo*>, ComparadorNodos> abiertos;

    // Mejor coste g conocido para cada posición.
    std::map<Posicion, int> mejoresCostes;

    // Posiciones que ya han sido inspeccionadas.
    std::set<Posicion> inspeccionados;

    // Posiciones generadas que siguen pendientes de inspección.
    std::set<Posicion> nodosGenerados;

    // Posiciones extraídas e inspeccionadas durante la búsqueda.
    std::set<Posicion> nodosInspeccionados;
};

#endif