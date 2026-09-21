#include "AEstrella.h"

#include <algorithm>
#include <cmath>

bool ComparadorNodos::operator()(const Nodo* izquierdo,
                                 const Nodo* derecho) const {
    if (izquierdo->getCosteTotal() != derecho->getCosteTotal()) {
        return izquierdo->getCosteTotal() > derecho->getCosteTotal();
    }
    if (izquierdo->getCosteHeuristico() != derecho->getCosteHeuristico()) {
        return izquierdo->getCosteHeuristico() > derecho->getCosteHeuristico();
    }
    return izquierdo->getPosicion() > derecho->getPosicion();
}

AEstrella::AEstrella(Mapa& mapa)
    : mapa(mapa) {}

int AEstrella::calcularHeuristica(const Posicion& posicion) const {
    const Posicion destino = mapa.getDestino();
        const int distancia = std::abs(posicion.first - destino.first) +
                              std::abs(posicion.second - destino.second);

        int costeMinimo = std::numeric_limits<int>::max();

        for (int fila = 0; fila < mapa.getFilas(); ++fila) {
            for (int columna = 0; columna < mapa.getColumnas(); ++columna) {
                int coste = mapa.obtenerNodo({fila, columna}).getCoste();

                if (coste == -1 || coste == 0) {
                    continue;
                }

                if (coste == 10) {
                    coste = 2;
                }

                costeMinimo = std::min(costeMinimo, coste);
            }
        }

        return distancia * costeMinimo;
}

std::vector<Posicion> AEstrella::reconstruirCamino(const Nodo* nodoFinal) const {
    std::vector<Posicion> camino;
    for (const Nodo* nodo = nodoFinal; nodo != nullptr; nodo = nodo->getPadre()) {
        camino.push_back(nodo->getPosicion());
    }
    std::reverse(camino.begin(), camino.end());
    return camino;
}

ResultadoBusqueda AEstrella::buscar() {
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
    mejoresCostes[origen] = 0;
    abiertos.push(&nodoOrigen);
    nodosGenerados.insert(origen);

    while (!abiertos.empty()) {
        Nodo* actual = abiertos.top();
        abiertos.pop();
        const Posicion posicionActual = actual->getPosicion();
        auto mejor = mejoresCostes.find(posicionActual);
        if (mejor == mejoresCostes.end() || actual->getCosteAcumulado() != mejor->second ||
            inspeccionados.find(posicionActual) != inspeccionados.end()) {
            continue;
        }
        nodosGenerados.erase(posicionActual);
        inspeccionados.insert(posicionActual);
        nodosInspeccionados.insert(posicionActual);
        if (posicionActual == destino) {
            std::vector<Posicion> camino = reconstruirCamino(actual);
            mapa.marcarCamino(camino);
            return {camino, actual->getCosteAcumulado(), nodosGenerados,
                    nodosInspeccionados, true};
        }

        for (const Posicion& vecinoPosicion : mapa.obtenerVecinos(posicionActual)) {
            Nodo& vecino = mapa.obtenerNodo(vecinoPosicion);
            int costeMovimiento = vecinoPosicion == destino ? 2 : vecino.getCoste();
            int nuevoCoste = actual->getCosteAcumulado() + costeMovimiento;
            auto costeConocido = mejoresCostes.find(vecinoPosicion);
            if (costeConocido != mejoresCostes.end() && nuevoCoste >= costeConocido->second) {
                continue;
            }
            mejoresCostes[vecinoPosicion] = nuevoCoste;
            vecino.actualizarCostes(nuevoCoste, calcularHeuristica(vecinoPosicion), actual);
            abiertos.push(&vecino);
            if (inspeccionados.find(vecinoPosicion) == inspeccionados.end()) {
                nodosGenerados.insert(vecinoPosicion);
            }
        }
    }

    mapa.marcarCamino({});
    return {{}, 0, nodosGenerados, nodosInspeccionados, false};
}