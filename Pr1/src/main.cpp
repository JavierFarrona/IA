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

void escribirResultado(std::ostream& salida, const std::string& instancia,
                       const Mapa& mapa, const ResultadoBusqueda& resultado) {
    salida << "Instancia n m co cd\n";
    const Posicion origen = mapa.getOrigen();
    const Posicion destino = mapa.getDestino();
    salida << instancia << ' ' << mapa.getFilas() << ' ' << mapa.getColumnas()
        << " co: (" << origen.first << ',' << origen.second << ")"
        << " cd: (" << destino.first << ',' << destino.second << ")\n";
    salida << "Camino " << formatearCamino(resultado.camino) << '\n';
    salida << "Coste " << resultado.coste << '\n';
    salida << "Nodos generados " << formatearPosiciones(resultado.nodosGenerados)
           << '\n';
    salida << "Nodos inspeccionados "
           << formatearPosiciones(resultado.nodosInspeccionados) << '\n';
}

} // namespace

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Uso: " << argv[0]
                  << " <mapa> [salida-mapa] [salida-resultados] [instancia]\n";
        return 2;
    }
    try {
        Mapa mapa;
        mapa.cargarDesdeFichero(argv[1]);
        AEstrella algoritmo(mapa);
        ResultadoBusqueda resultado = algoritmo.buscar();
        std::string instancia = argc >= 5
                                    ? argv[4]
                                    : std::filesystem::path(argv[1]).stem().string();
        std::string salidaMapa = argc >= 3 ? argv[2] : instancia + "_salida.txt";
        std::string salidaResultados = argc >= 4 ? argv[3] : "resultados.txt";
        mapa.guardarConCamino(salidaMapa);
        std::ofstream ficheroResultados(salidaResultados);
        if (!ficheroResultados) {
            throw std::runtime_error("No se pudo crear el fichero: " + salidaResultados);
        }
        escribirResultado(std::cout, instancia, mapa, resultado);
        escribirResultado(ficheroResultados, instancia, mapa, resultado);
        return resultado.encontrado ? 0 : 1;
    } catch (const std::exception& excepcion) {
        std::cerr << "Error: " << excepcion.what() << '\n';
        return 2;
    }
}