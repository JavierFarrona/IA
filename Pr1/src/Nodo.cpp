#include "Nodo.h"

Nodo::Nodo()
    : posicion(0, 0), coste(-1), esCamino(false), costeAcumulado(0),
      costeHeuristico(0), padre(nullptr) {}

Nodo::Nodo(int fila, int columna, int coste)
    : posicion(fila, columna), coste(coste), esCamino(false),
      costeAcumulado(0), costeHeuristico(0), padre(nullptr) {}

Posicion Nodo::getPosicion() const { return posicion; }
int Nodo::getFila() const { return posicion.first; }
int Nodo::getColumna() const { return posicion.second; }
int Nodo::getCoste() const { return coste; }
int Nodo::getCosteAcumulado() const { return costeAcumulado; }
int Nodo::getCosteHeuristico() const { return costeHeuristico; }
int Nodo::getCosteTotal() const { return costeAcumulado + costeHeuristico; }
Nodo* Nodo::getPadre() const { return padre; }
bool Nodo::esObstaculo() const { return coste < 0; }
bool Nodo::esTransitable() const { return !esObstaculo(); }
bool Nodo::perteneceAlCamino() const { return esCamino; }

void Nodo::actualizarCostes(int costeAcumuladoNuevo,
                            int costeHeuristicoNuevo, Nodo* padreNuevo) {
    costeAcumulado = costeAcumuladoNuevo;
    costeHeuristico = costeHeuristicoNuevo;
    padre = padreNuevo;
}

void Nodo::marcarCamino(bool esCaminoNuevo) { esCamino = esCaminoNuevo; }

void Nodo::reiniciarBusqueda() {
    esCamino = false;
    costeAcumulado = 0;
    costeHeuristico = 0;
    padre = nullptr;
}