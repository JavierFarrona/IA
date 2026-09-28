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
* Archivo: AEstrella.cpp
* Referencias: 
*     Enunciado de la práctica
* Historial de revisiones
*     20 Sep 2026 - Creación (primera versión) del código
*     22 Sep 2026 - Completa implementación de la clase AEstrella y pruebas iniciales
*     23 Sep 2026 - Documentacion del código
*/

#include "AEstrella.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

namespace {

// Devuelve el símbolo gráfico asociado a cada celda para la visualización
// didáctica del algoritmo.
std::string simboloCasilla(const Mapa& mapa, const Posicion& posicion,
                          const std::set<Posicion>& camino) {
    const Posicion origen = mapa.getOrigen();
    const Posicion destino = mapa.getDestino();
    const Nodo& nodo = mapa.obtenerNodo(posicion);

    if (posicion == origen) {
        return "🤖";
    }
    if (posicion == destino) {
        return "🏁";
    }
    if (nodo.getCoste() < 0) {
        return "🟥";
    }
    if (camino.find(posicion) != camino.end()) {
        return "🟩";
    }

    const int coste = nodo.getCoste();
    if (coste <= 1) {
        return "⬜";
    }
    if (coste <= 3) {
        return "◽";
    }
    if (coste <= 5) {
        return "◾";
    }
    if (coste <= 7) {
        return "◼️";
    }
    return "⬛";
}

// Formatea un conjunto de posiciones para imprimirlo como una lista de
// coordenadas del tipo (fila,columna) separadas por comas.
std::string formatearConjunto(const std::set<Posicion>& posiciones) {
    if (posiciones.empty()) {
        return "";
    }

    std::ostringstream salida;
    bool primera = true;
    for (const Posicion& posicion : posiciones) {
        if (!primera) {
            salida << ", ";
        }
        salida << '(' << posicion.first << ',' << posicion.second << ')';
        primera = false;
    }
    return salida.str();
}

// Muestra el estado actual de la frontera y los nodos ya cerrados.
void imprimirEstadoIteracion(int iteracion, const std::set<Posicion>& abiertos,
                            const std::set<Posicion>& cerrados,
                            std::ostream& salida) {
    salida << "Iteración " << iteracion << '\n';
    salida << "-----------\n";
    salida << "Abiertos = " << formatearConjunto(abiertos) << '\n';
    salida << "Cerrados = " << formatearConjunto(cerrados) << '\n';
    salida << "------------------------\n";
}

// Muestra el mapa con los colores/símbolos del algoritmo para el paso a paso.
void imprimirMapaPaso(const Mapa& mapa, const std::set<Posicion>& camino,
                      std::ostream& salida) {
    salida << "\nMapa actual:\n";
    for (int fila = 0; fila < mapa.getFilas(); ++fila) {
        for (int columna = 0; columna < mapa.getColumnas(); ++columna) {
            const Posicion posicion(fila, columna);
            salida << simboloCasilla(mapa, posicion, camino) << ' ';
        }
        salida << '\n';
    }
}

// Pausa la ejecución solo para la consola interactiva del modo paso a paso.
void esperarPaso(std::ostream* salida) {
    if (salida == nullptr || salida != &std::cout) {
        return;
    }
    std::cout << "Pulsa Intro para avanzar al siguiente paso..." << std::flush;
    std::cin.get();
    std::cout << '\n';
}

}  // namespace

// Ordena los nodos por la prioridad de A* usando f(n), luego h(n) y, por último,
// la posición para fijar el orden en casos de empate.
bool ComparadorNodos::operator()(const Nodo* izquierdo,
                                 const Nodo* derecho) const {
    // El primer criterio representa f(n)=g(n)+h(n), es decir, el coste ya
    // recorrido más la estimación restante hasta el objetivo.
    if (izquierdo->getCosteTotal() != derecho->getCosteTotal()) {
        return izquierdo->getCosteTotal() > derecho->getCosteTotal();
    }
    if (izquierdo->getCosteHeuristico() != derecho->getCosteHeuristico()) {
        return izquierdo->getCosteHeuristico() > derecho->getCosteHeuristico();
    }
    return izquierdo->getPosicion() > derecho->getPosicion();
}

AEstrella::AEstrella(Mapa& mapa) : mapa(mapa) {}

// Calcula la distancia Manhattan entre la posición actual y el destino y la
// pondera por el menor coste transitado del mapa para obtener una heurística
// más informativa.
int AEstrella::calcularHeuristica(const Posicion& posicion) const {
    const Posicion destino = mapa.getDestino();
    const int distancia = std::abs(posicion.first - destino.first) +
                          std::abs(posicion.second - destino.second);

    // Se usa Manhattan porque solo se permiten desplazamientos verticales y
    // horizontales. Multiplicarla por el menor coste evita sobreestimar el
    // coste de cualquier ruta posible.
    int costeMinimo = std::numeric_limits<int>::max();

    for (int fila = 0; fila < mapa.getFilas(); ++fila) {
        for (int columna = 0; columna < mapa.getColumnas(); ++columna) {
            const int coste = mapa.obtenerNodo({fila, columna}).getCoste();

            // Obstáculos y origen no son costes de desplazamiento normales.
            if (coste == -1 || coste == 0) {
                continue;
            }

            int costeValido = coste;
            // El destino se almacena como 10, pero entrar en él cuesta 2,
            // igual que establece el contrato del problema.
            if (costeValido == 10) {
                costeValido = 2;
            }

            costeMinimo = std::min(costeMinimo, costeValido);
        }
    }

    return distancia * costeMinimo;
}

// Reconstituye la ruta solución siguiendo los punteros a padre desde el nodo
// final hasta el origen.
std::vector<Posicion> AEstrella::reconstruirCamino(const Nodo* nodoFinal) const {
    std::vector<Posicion> camino;
    // Cada nodo conoce al anterior; se recorre hacia atrás y después se
    // invierte la secuencia para devolverla desde el origen al destino.
    for (const Nodo* nodo = nodoFinal; nodo != nullptr; nodo = nodo->getPadre()) {
        camino.push_back(nodo->getPosicion());
    }
    std::reverse(camino.begin(), camino.end());
    return camino;
}

// Ejecuta la búsqueda A* completa sobre el mapa, actualizando la frontera,
// revisando los vecinos y devolviendo la información del resultado. Cuando
// pasoAPaso está activado, la función pausa la ejecución para inspeccionar el
// estado actual del algoritmo y el mapa en cada expansión.
ResultadoBusqueda AEstrella::buscar(bool pasoAPaso, std::ostream* salida,
                                   bool mostrarMapa) {
    // La instancia puede reutilizarse: se vacían las estructuras globales de
    // esta búsqueda y se borran los datos temporales guardados en cada nodo.
    while (!abiertos.empty()) {
        abiertos.pop();
    }
    mejoresCostes.clear();
    inspeccionados.clear();
    nodosGenerados.clear();
    nodosInspeccionados.clear();

    for (int fila = 0; fila < mapa.getFilas(); ++fila) {
        for (int columna = 0; columna < mapa.getColumnas(); ++columna) {
            mapa.obtenerNodo({fila, columna}).reiniciarBusqueda();
        }
    }

    const Posicion origen = mapa.getOrigen();
    const Posicion destino = mapa.getDestino();
    Nodo& nodoOrigen = mapa.obtenerNodo(origen);
    nodoOrigen.actualizarCostes(0, calcularHeuristica(origen), nullptr);
    // El origen tiene g=0, y su h inicial determina su prioridad en abiertos.
    mejoresCostes[origen] = 0;
    abiertos.push(&nodoOrigen);
    nodosGenerados.insert(origen);

    int iteracion = 0;
    if (pasoAPaso && salida != nullptr) {
        imprimirEstadoIteracion(iteracion, nodosGenerados, nodosInspeccionados, *salida);
    }

    while (!abiertos.empty()) {
        // La cola proporciona el candidato con menor f. Puede contener una
        // versión vieja si la misma posición fue mejorada posteriormente.
        Nodo* actual = abiertos.top();
        abiertos.pop();
        const Posicion posicionActual = actual->getPosicion();
        auto mejor = mejoresCostes.find(posicionActual);

        if (mejor == mejoresCostes.end() ||
            actual->getCosteAcumulado() != mejor->second ||
            inspeccionados.find(posicionActual) != inspeccionados.end()) {
            continue;
        }

        nodosGenerados.erase(posicionActual);
        // Al extraerlo pasa de abiertos a inspeccionados y ya puede expandirse.
        inspeccionados.insert(posicionActual);
        nodosInspeccionados.insert(posicionActual);

        if (pasoAPaso && salida != nullptr) {
            ++iteracion;
            imprimirEstadoIteracion(iteracion, nodosGenerados, nodosInspeccionados, *salida);
            if (mostrarMapa) {
                std::set<Posicion> caminoActual;
                for (const auto& posicion : nodosInspeccionados) {
                    caminoActual.insert(posicion);
                }
                imprimirMapaPaso(mapa, caminoActual, *salida);
            }
            if (salida == &std::cout) {
                esperarPaso(salida);
            }
        }

        if (posicionActual == destino) {
            std::vector<Posicion> camino = reconstruirCamino(actual);
            mapa.marcarCamino(camino);
            if (pasoAPaso && salida != nullptr) {
                *salida << "Camino: ";
                for (std::size_t indice = 0; indice < camino.size(); ++indice) {
                    if (indice > 0) {
                        *salida << " -> ";
                    }
                    *salida << "(" << camino[indice].first << ","
                            << camino[indice].second << ")";
                }
                *salida << '\n';
                *salida << "Coste: " << actual->getCosteAcumulado() << '\n';
            }
            return {camino, actual->getCosteAcumulado(), nodosGenerados,
                    nodosInspeccionados, true};
        }

        for (const Posicion& vecinoPosicion : mapa.obtenerVecinos(posicionActual)) {
            // Solo se recorren vecinos válidos y transitables. El coste de
            // movimiento es el de la celda de llegada, salvo el destino.
            Nodo& vecino = mapa.obtenerNodo(vecinoPosicion);
            const int costeMovimiento = vecinoPosicion == destino ? 2 : vecino.getCoste();
            const int nuevoCoste = actual->getCosteAcumulado() + costeMovimiento;
            const auto costeConocido = mejoresCostes.find(vecinoPosicion);

            if (costeConocido != mejoresCostes.end() &&
                nuevoCoste >= costeConocido->second) {
                // Una ruta que no mejora g(n) no debe sustituir al padre ni
                // introducir otra copia útil en la frontera.
                continue;
            }

            mejoresCostes[vecinoPosicion] = nuevoCoste;
            vecino.actualizarCostes(nuevoCoste,
                                    calcularHeuristica(vecinoPosicion), actual);
            abiertos.push(&vecino);
            // Se vuelve a insertar aunque exista una copia anterior: la
            // comprobación de coste obsoleto filtra esa copia al extraerla.

            if (inspeccionados.find(vecinoPosicion) == inspeccionados.end()) {
                nodosGenerados.insert(vecinoPosicion);
            }
        }
    }

    if (pasoAPaso && salida != nullptr) {
        *salida << "No se encontró ruta válida hasta el destino.\n";
    }

    mapa.marcarCamino({});
    // Si abiertos se agota, no existe ruta transitable desde el origen.
    return {{}, 0, nodosGenerados, nodosInspeccionados, false};
}