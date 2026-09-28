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
* Archivo: Nodo.h
* Referencias: 
*     Enunciado de la práctica
* Historial de revisiones
*     20 Sep 2026 - Creación (primera versión) del código
*     22 Sep 2026 - Completa implementación de la clase Nodo y pruebas iniciales
*     23 Sep 2026 - Documentacion del código
*/

#ifndef NODO_H
#define NODO_H

#include <utility>

// Representa una posición del mapa mediante la coordenada (fila, columna).
using Posicion = std::pair<int, int>;

// Un nodo encapsula el estado de una celda dentro del problema de búsqueda.
// Guarda su posición, el coste de la celda, la información del camino actual y
// los valores de la función A*.
class Nodo {
 public:
   // Crea un nodo en estado inicial con valores por defecto de búsqueda.
   // Su coste -1 lo deja en estado de obstáculo hasta que se configure.
    Nodo();

    // Crea un nodo con una posición concreta y el coste de acceso a esa celda.
    Nodo(int fila, int columna, int coste);

    // Devuelve la posición del nodo como un par (fila, columna).
    Posicion getPosicion() const;

    // Devuelve la fila del nodo.
    int getFila() const;

    // Devuelve la columna del nodo.
    int getColumna() const;

    // Devuelve el coste de paso de la celda.
    int getCoste() const;

    // Devuelve el coste acumulado desde el origen, g(n).
    int getCosteAcumulado() const;

    // Devuelve la estimación del coste restante hasta el destino, h(n).
    int getCosteHeuristico() const;

   // Devuelve el valor de evaluación total de la búsqueda, f(n) = g(n) + h(n).
   // A* extrae primero el nodo cuyo valor f es menor.
    int getCosteTotal() const;

    // Devuelve el nodo padre usado para reconstruir el camino.
    Nodo* getPadre() const;

    // Indica si la celda es un obstáculo y, por tanto, no es transitables.
    bool esObstaculo() const;

    // Indica si la celda puede ser recorrida por el agente.
    bool esTransitable() const;

    // Indica si la celda forma parte del camino solución actualmente marcado.
    bool perteneceAlCamino() const;

   // Actualiza los valores de costes y enlaza el nodo con su padre.
   // El padre conserva la decisión que permitió obtener el mejor g(n).
    void actualizarCostes(int costeAcumulado, int costeHeuristico, Nodo* padre);

    // Marca o desmarca la celda como parte del camino final.
    void marcarCamino(bool esCamino);

   // Restablece los datos temporales del nodo antes de iniciar una nueva
   // búsqueda, sin modificar su posición ni el coste del terreno.
    void reiniciarBusqueda();

 private:
    // Coordenadas del nodo dentro del mapa.
    Posicion posicion;

    // Coste asociado a atravesar la celda. Un valor -1 representa un obstáculo.
    int coste;

    // Indica si la celda está marcada como parte del camino solución.
    bool esCamino;

    // Coste acumulado desde el origen hasta este nodo, g(n).
    int costeAcumulado;

    // Estimación del coste restante hasta el destino, h(n).
    int costeHeuristico;

    // Nodo padre utilizado para reconstruir la ruta del origen al destino.
    Nodo* padre;
};

#endif