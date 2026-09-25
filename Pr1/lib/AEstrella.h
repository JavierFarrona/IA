/**
* Universidad de La Laguna
* Escuela Superior de Ingeniería y Tecnología
* Grado en Ingeniería Informática
* Asignatura: Inteligencia Artificial
* Curso: 3º
* C:\Users\javie\Desktop\Práctica\IA\Pr1\lib
* Autor: Javier Farrona Cabrera
* Correo: alu0101541983@ull.edu.es
* Fecha 23 Sep 2026
* Archivo: AEstrella.h
* Referencias: 
*     Enunciado de la práctica
* Historial de revisiones
*     20 Sep 2026 - Creación (primera versión) del código
*     22 Sep 2026 - Implementación de la búsqueda A* y pruebas iniciales
*     23 Sep 2026 - Documentacion del código
*/


#ifndef A_ESTRELLA_H
#define A_ESTRELLA_H

#include "Mapa.h"

#include <iosfwd>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <vector>

// Ordena primero por el coste total f(n) y, en caso de empate, por la heurística
// y finalmente por la posición para mantener una prioridad determinista.
struct ComparadorNodos {
    // En priority_queue, devolver true significa que izquierdo tiene menor
    // prioridad; por eso se invierte la comparación habitual de los costes.
    bool operator()(const Nodo* izquierdo, const Nodo* derecho) const;
};

// Resultado devuelto por la búsqueda A* con el camino y las métricas asociadas.
struct ResultadoBusqueda {
    // Secuencia de posiciones que forma el camino solución.
    std::vector<Posicion> camino;

    // Coste total acumulado del camino encontrado; vale 0 si no hay camino.
    int coste;

    // Posiciones que se han generado y aún no han sido extraídas de la frontera.
    std::set<Posicion> nodosGenerados;

    // Posiciones ya inspeccionadas durante la búsqueda.
    std::set<Posicion> nodosInspeccionados;

    // Indica si se ha encontrado una ruta válida hasta el destino.
    bool encontrado;
};

// Implementa la búsqueda informada A* sobre un mapa bidimensional.
class AEstrella {
 public:
    // Construye un algoritmo A* asociado a un mapa concreto.
    explicit AEstrella(Mapa& mapa);

    // Ejecuta la búsqueda y devuelve el camino, el coste y el estado final.
    // Si se activa el modo paso a paso, se muestran las decisiones relevantes del
    // algoritmo en la salida indicada.
    ResultadoBusqueda buscar(bool pasoAPaso = false, std::ostream* salida = nullptr);

    // Calcula la heurística de Manhattan ponderada por el coste mínimo del mapa.
    // La distancia Manhattan supone movimientos ortogonales, sin diagonales.
    int calcularHeuristica(const Posicion& posicion) const;

 private:
    // Reconstruye el camino desde el nodo final hasta el origen usando los padres.
    std::vector<Posicion> reconstruirCamino(const Nodo* nodoFinal) const;

    // Mapa sobre el que se ejecuta la búsqueda.
    Mapa& mapa;

    // Cola de prioridad con los nodos abiertos pendientes de expansión.
    std::priority_queue<Nodo*, std::vector<Nodo*>, ComparadorNodos> abiertos;

    // Mejor coste g(n) conocido para cada posición del mapa. Permite descartar
    // copias antiguas de un nodo que permanezcan dentro de la cola de abiertos.
    std::map<Posicion, int> mejoresCostes;

    // Posiciones con alguna entrada pendiente o ya considerada para controlar
    // duplicados durante la búsqueda.
    std::set<Posicion> inspeccionados;

    // Posiciones en la frontera de búsqueda todavía pendientes de expansión.
    std::set<Posicion> nodosGenerados;

    // Posiciones extraídas y procesadas por la búsqueda.
    std::set<Posicion> nodosInspeccionados;
};

#endif