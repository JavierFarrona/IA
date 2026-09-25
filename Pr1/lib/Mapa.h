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
* Archivo: Mapa.h
* Referencias: 
*     Enunciado de la práctica
* Historial de revisiones
*     20 Sep 2026 - Creación (primera versión) del código
*     22 Sep 2026 - Completa implementación de la clase Mapa y pruebas iniciales
*     23 Sep 2026 - Documentacion del código
*/

#ifndef MAPA_H
#define MAPA_H

#include "Nodo.h"

#include <string>
#include <vector>

// Representa el entorno sobre el que se ejecuta la búsqueda del robot.
// Cada celda tiene un coste asociado, puede ser libre, obstáculo o punto de
// origen/destino, y se utiliza para generar vecinos y guardar la solución.
class Mapa {
 public:
    // Crea un mapa vacío con dimensiones nulas.
    Mapa();

    // Crea un mapa rectangular con las dimensiones especificadas.
    Mapa(int filas, int columnas);

   // Carga la estructura del mapa desde un fichero de texto. El valor -1
   // representa un obstáculo, 0 el origen y 10 el destino.
    void cargarDesdeFichero(const std::string& nombreFichero);

   // Guarda el mapa en disco y marca la ruta encontrada con asteriscos.
   // El fichero se escribe con los costes originales en las demás celdas.
    void guardarConCamino(const std::string& nombreFichero) const;

    // Comprueba si una posición pertenece al rango válido del mapa.
    bool esValida(const Posicion& posicion) const;

    // Comprueba si una posición es válida y transitada por el robot.
    bool esTransitable(const Posicion& posicion) const;

   // Devuelve las posiciones vecinas transitables en las cuatro direcciones.
   // Las posiciones fuera de los límites y los obstáculos se filtran aquí.
    std::vector<Posicion> obtenerVecinos(const Posicion& posicion) const;

    // Devuelve la referencia constante al nodo asociado a una posición.
    const Nodo& obtenerNodo(const Posicion& posicion) const;

    // Devuelve la referencia modificable al nodo asociado a una posición.
    Nodo& obtenerNodo(const Posicion& posicion);

    // Devuelve el número de filas del mapa.
    int getFilas() const;

    // Devuelve el número de columnas del mapa.
    int getColumnas() const;

    // Devuelve la posición inicial del robot.
    Posicion getOrigen() const;

    // Devuelve la posición objetivo del robot.
    Posicion getDestino() const;

   // Marca como parte del camino las posiciones indicadas en la solución.
   // Primero limpia marcas anteriores para que una búsqueda sea reutilizable.
    void marcarCamino(const std::vector<Posicion>& camino);

 private:
    // Número de filas del entorno.
    int filas;

    // Número de columnas del entorno.
    int columnas;

    // Matriz de nodos que representa cada celda del mapa.
    std::vector<std::vector<Nodo>> nodos;

    // Posición inicial del robot.
    Posicion origen;

    // Posición que debe alcanzar el robot.
    Posicion destino;
};

#endif