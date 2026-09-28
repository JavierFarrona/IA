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
* Archivo: main.cpp
* Referencias: 
*     Enunciado de la práctica
* Historial de revisiones
*     20 Sep 2026 - Creación (primera versión) del código
*     22 Sep 2026 - Union de todas las partes del proyecto y pruebas iniciales
*     23 Sep 2026 - Documentacion del código
*/

#include "AEstrella.h"

#include <exception>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

namespace {

// Formatea una ruta como una secuencia de coordenadas (fila,columna).
std::string formatearCamino(const std::vector<Posicion>& camino) {
    if (camino.empty()) {
        return "-";
    }
    std::ostringstream salida;
    for (std::size_t indice = 0; indice < camino.size(); ++indice) {
        if (indice > 0) {
            salida << '-';
        }
        salida << '(' << camino[indice].first << ',' << camino[indice].second << ')';
    }
    return salida.str();
}

// Formatea un conjunto de posiciones para mostrarlo en la salida textual.
std::string formatearPosiciones(const std::set<Posicion>& posiciones) {
    if (posiciones.empty()) {
        return "-";
    }
    std::ostringstream salida;
    bool primera = true;
    for (const Posicion& posicion : posiciones) {
        if (!primera) {
            salida << ' ';
        }
        salida << '(' << posicion.first << ',' << posicion.second << ')';
        primera = false;
    }
    return salida.str();
}

// Asigna un símbolo visual a cada celda según el tipo de contenido:
// robot, meta, obstáculo, camino o coste del terreno.
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

// Dibuja el mapa completo con emojis y tonos de gris para que sea más fácil
// visualizar la solución final y la distribución del problema.
void imprimirMapaVisual(std::ostream& salida, const Mapa& mapa,
                        const ResultadoBusqueda& resultado) {
    salida << "\nMapa visual:\n";

    for (int fila = 0; fila < mapa.getFilas(); ++fila) {
        for (int columna = 0; columna < mapa.getColumnas(); ++columna) {
            const Posicion posicion(fila, columna);
            salida << simboloCasilla(mapa, posicion, std::set<Posicion>(
                                                     resultado.camino.begin(),
                                                     resultado.camino.end()))
                   << ' ';
        }
        salida << '\n';
    }
}

// Genera el resumen textual del resultado con el formato canónico del
// proyecto para que tanto la consola como el fichero de resultados compartan
// exactamente la misma salida.
std::string generarResumenResultado(const std::string& instancia,
                                  const Mapa& mapa,
                                  const ResultadoBusqueda& resultado) {
    std::ostringstream buffer;
    buffer << "Instancia n m co cd\n";
    const Posicion origen = mapa.getOrigen();
    const Posicion destino = mapa.getDestino();
    buffer << instancia << ' ' << mapa.getFilas() << ' ' << mapa.getColumnas()
           << " co: (" << origen.first << ',' << origen.second << ")"
           << " cd: (" << destino.first << ',' << destino.second << ")\n";
    buffer << "Camino " << formatearCamino(resultado.camino) << '\n';
    buffer << "Coste " << resultado.coste << '\n';
    buffer << "Nodos generados " << formatearPosiciones(resultado.nodosGenerados)
           << '\n';
    buffer << "Nodos inspeccionados "
           << formatearPosiciones(resultado.nodosInspeccionados) << '\n';
    return buffer.str();
}

// Escribe el resumen textual del resultado con una salida estable y reproducible.
void escribirResultado(std::ostream& salida, const std::string& instancia,
                       const Mapa& mapa, const ResultadoBusqueda& resultado) {
    salida << generarResumenResultado(instancia, mapa, resultado);
}

} // namespace

// Punto de entrada del programa. Permite ejecutar la búsqueda en modo directo
// o en modo paso a paso, según el parámetro indicado por la línea de comandos.
int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0]
                  << " <mapa> [salida-mapa] [salida-resultados] [instancia] [--step|--direct]\n";
        return 2;
    }

    try {
        // Los argumentos posicionales definen ficheros e identificador; las
        // banderas solo seleccionan la modalidad de ejecución.
        bool pasoAPaso = false;
        std::vector<std::string> argumentosPosicionales;

        for (int indice = 2; indice < argc; ++indice) {
            const std::string argumento = argv[indice];
            if (argumento == "--step" || argumento == "-s") {
                pasoAPaso = true;
            } else if (argumento == "--direct" || argumento == "-d") {
                pasoAPaso = false;
            } else {
                argumentosPosicionales.push_back(argumento);
            }
        }

        std::string salidaMapa;
        std::string salidaResultados;
        std::string instancia;

        if (!argumentosPosicionales.empty()) {
            salidaMapa = argumentosPosicionales[0];
        }
        if (argumentosPosicionales.size() >= 2) {
            salidaResultados = argumentosPosicionales[1];
        }
        if (argumentosPosicionales.size() >= 3) {
            instancia = argumentosPosicionales[2];
        }

        // La carga puede lanzar una excepción si el fichero no es válido; el
        // bloque catch la convierte en un mensaje de error y código 2.
        Mapa mapa;
        mapa.cargarDesdeFichero(argv[1]);

        // Los nombres omitidos se derivan de la instancia para permitir una
        // ejecución mínima indicando únicamente el mapa de entrada.
        if (salidaMapa.empty()) {
            instancia = instancia.empty() ? std::filesystem::path(argv[1]).stem().string() : instancia;
            salidaMapa = instancia + "_salida.txt";
        }
        if (salidaResultados.empty()) {
            salidaResultados = "resultados.txt";
        }
        if (instancia.empty()) {
            instancia = std::filesystem::path(argv[1]).stem().string();
        }

        // La solución se presenta tanto por consola como en el fichero de
        // resultados. En modo paso a paso se escribe la traza de iteraciones
        // al fichero y a la consola; en modo normal se usa el resumen clásico.
        std::ofstream ficheroResultados(salidaResultados);
        if (!ficheroResultados) {
            throw std::runtime_error("No se pudo crear el fichero: " + salidaResultados);
        }

        AEstrella algoritmo(mapa);
        std::ostringstream traza;

        if (pasoAPaso) {
            ResultadoBusqueda resultado = algoritmo.buscar(true, &std::cout, true);
            AEstrella algoritmoArchivo(mapa);
            algoritmoArchivo.buscar(true, &traza, false);
            mapa.guardarConCamino(salidaMapa);
            ficheroResultados << traza.str();
            return resultado.encontrado ? 0 : 1;
        }

        ResultadoBusqueda resultado = algoritmo.buscar(false, nullptr);
        AEstrella algoritmoArchivo(mapa);
        algoritmoArchivo.buscar(true, &traza, false);
        mapa.guardarConCamino(salidaMapa);
        ficheroResultados << traza.str();

        imprimirMapaVisual(std::cout, mapa, resultado);
        escribirResultado(std::cout, instancia, mapa, resultado);
        return resultado.encontrado ? 0 : 1;
    } catch (const std::exception& excepcion) {
        std::cerr << "Error: " << excepcion.what() << '\n';
        return 2;
    }
}