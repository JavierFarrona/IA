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
* Archivo: Nodo.cpp
* Referencias: 
*     Enunciado de la práctica
* Historial de revisiones
*     20 Sep 2026 - Creación (primera versión) del código
*     22 Sep 2026 - Completa implementación de la clase Nodo y pruebas iniciales
*     23 Sep 2026 - Documentacion del código
*/

#include "Nodo.h"

// Crea un nodo en un estado neutro, útil para inicializaciones temporales.
Nodo::Nodo()
    : posicion(0, 0), coste(-1), esCamino(false), costeAcumulado(0),
      costeHeuristico(0), padre(nullptr) {}

// Crea un nodo asociado a una celda concreta del mapa con un coste fijo.
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

// Una celda con coste negativo se interpreta como obstáculo.
bool Nodo::esObstaculo() const { return coste < 0; }

// Una celda no bloqueada puede ser recorrida por el robot.
bool Nodo::esTransitable() const { return !esObstaculo(); }

// Indica si esta celda está marcada como parte de la ruta final.
bool Nodo::perteneceAlCamino() const { return esCamino; }

// Actualiza los datos del nodo necesarios para la expansión de A*.
void Nodo::actualizarCostes(int costeAcumuladoNuevo,
                            int costeHeuristicoNuevo, Nodo* padreNuevo) {
    // El nuevo padre permite conservar la mejor ruta conocida para reconstruir
    // el camino cuando este nodo sea elegido por A*.
    costeAcumulado = costeAcumuladoNuevo;
    costeHeuristico = costeHeuristicoNuevo;
    padre = padreNuevo;
}

// Cambia el estado visual del nodo para indicar si forma parte de la solución.
void Nodo::marcarCamino(bool esCaminoNuevo) { esCamino = esCaminoNuevo; }

// Reinicia los valores temporales del nodo antes de cada nueva ejecución.
void Nodo::reiniciarBusqueda() {
    // Se mantienen los datos estáticos de la celda y se limpian solo los datos
    // derivados de una ejecución concreta del algoritmo.
    esCamino = false;
    costeAcumulado = 0;
    costeHeuristico = 0;
    padre = nullptr;
}