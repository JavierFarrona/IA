// Guia de implementacion de las modificaciones para Pr1.
// Este archivo es un documento: los fragmentos estan dentro de #if 0 y no se compilan.
// Cada bloque indica el archivo y el lugar donde insertar o sustituir el codigo.
// Copiar los fragmentos necesarios al archivo indicado y retirar sus envolturas #if 0.
//
// Compilar desde Pr1 con: make
// IMPORTANTE: los comandos indicados abajo son la interfaz objetivo. El main.cpp
// actual solo reconoce --step y --direct; para usar las nuevas opciones hay que
// parsearlas en main.cpp y pasarlas a AEstrella::buscar(). En WSL/Linux se ejecuta
// como ./build/pr1; en Windows normalmente como build/pr1.exe.

//MODI[1]: Permitir movimientos diagonales.
//Como funciona: se agregan los ocho desplazamientos posibles. Antes de aceptar
//una diagonal se comprueba que las dos casillas ortogonales contiguas sean libres,
//evitando pasar por la esquina de un obstaculo. El coste diagonal se aproxima a
//sqrt(2) veces el coste de entrada y la heuristica pasa a distancia octile.
//Ejecucion objetivo: ./build/pr1 test/entorno01.txt salida.txt resultados.txt MODI1 --diagonal
//Ubicacion: Pr1/lib/Mapa.h, sustituir la declaracion de obtenerVecinos.
#if 0
std::vector<Posicion> obtenerVecinos(const Posicion& posicion,
                                     bool permitirDiagonales = false) const;
#endif

//MODI[1]: Sustituir Mapa::obtenerVecinos en Pr1/src/Mapa.cpp.
//Las comprobaciones laterales impiden pasar diagonalmente entre dos obstaculos.
#if 0
std::vector<Posicion> Mapa::obtenerVecinos(const Posicion& posicion,
                                           bool permitirDiagonales) const {
    static const int desplazamientos[][2] = {
        {-1, 0}, {0, 1}, {1, 0}, {0, -1},
        {-1, -1}, {-1, 1}, {1, 1}, {1, -1}};
    std::vector<Posicion> vecinos;
    const int cantidad = permitirDiagonales ? 8 : 4;

    for (int indice = 0; indice < cantidad; ++indice) {
        const int deltaFila = desplazamientos[indice][0];
        const int deltaColumna = desplazamientos[indice][1];
        const Posicion vecino(posicion.first + deltaFila,
                             posicion.second + deltaColumna);
        if (!esTransitable(vecino)) {
            continue;
        }

        if (indice >= 4) {
            const Posicion lateralFila(posicion.first + deltaFila,
                                       posicion.second);
            const Posicion lateralColumna(posicion.first,
                                          posicion.second + deltaColumna);
            if (!esTransitable(lateralFila) || !esTransitable(lateralColumna)) {
                continue;
            }
        }
        vecinos.push_back(vecino);
    }
    return vecinos;
}
#endif

//MODI[1]: En Pr1/src/AEstrella.cpp, sustituir la llamada que obtiene vecinos.
#if 0
for (const Posicion& vecinoPosicion :
     mapa.obtenerVecinos(posicionActual, permitirDiagonales)) {
    // Mantener dentro de este bucle el procesamiento existente del vecino.
}
#endif

//MODI[1]: En el mismo bucle de AEstrella.cpp, sustituir el calculo de coste.
//Añadir #include <cmath> al principio del archivo.
#if 0
const bool movimientoDiagonal =
    vecinoPosicion.first != posicionActual.first &&
    vecinoPosicion.second != posicionActual.second;
const int costeEntrada = vecinoPosicion == destino ? 2 : vecino.getCoste();
const int costeMovimiento = movimientoDiagonal
    ? static_cast<int>(std::ceil(std::sqrt(2.0) * costeEntrada))
    : costeEntrada;
const int nuevoCoste = actual->getCosteAcumulado() + costeMovimiento;
#endif

//MODI[1]: En calcularHeuristica, usar heuristica octile cuando haya diagonales.
//costeMinimo es el menor coste de entrada transitable, contando el destino como 2.
#if 0
const int deltaFila = std::abs(posicion.first - destino.first);
const int deltaColumna = std::abs(posicion.second - destino.second);
const int diagonales = std::min(deltaFila, deltaColumna);
const int rectas = std::max(deltaFila, deltaColumna) - diagonales;
const int costeDiagonalMinimo =
    static_cast<int>(std::ceil(std::sqrt(2.0) * costeMinimo));
const int heuristica = permitirDiagonales
    ? diagonales * costeDiagonalMinimo + rectas * costeMinimo
    : (deltaFila + deltaColumna) * costeMinimo;
#endif

//MODI[2]: Desempatar cuando dos nodos tienen el mismo f(n).
//Como funciona: si f(a)==f(b), se expande primero el nodo con menor h(n), que
//estima estar mas cerca del objetivo. Si h tambien empata, se usa la coordenada
//para mantener un orden determinista. No necesita una opcion CLI propia.
//Ejecucion: ./build/pr1 test/entorno01.txt salida.txt resultados.txt MODI2
//Ubicacion: Pr1/src/AEstrella.cpp, en ComparadorNodos::operator(), despues
//de comparar getCosteTotal() y antes del desempate final por getPosicion().
//La menor h(n) suele favorecer nodos mas cercanos al objetivo; la posicion hace
//que el resultado continue siendo determinista si tambien empata h(n).
#if 0
if (izquierdo->getCosteTotal() != derecho->getCosteTotal()) {
    return izquierdo->getCosteTotal() > derecho->getCosteTotal();
}
//MODI[2]: Este es el criterio de desempate para f(n) iguales.
if (izquierdo->getCosteHeuristico() != derecho->getCosteHeuristico()) {
    return izquierdo->getCosteHeuristico() > derecho->getCosteHeuristico();
}
return izquierdo->getPosicion() > derecho->getPosicion();
#endif

//MODI[3]: Obligar a pasar por checkpoints en el orden indicado.
//Como funciona: A* calcula un tramo hasta cada checkpoint, en orden, y el ultimo
//tramo llega al destino. Los tramos se concatenan sin repetir el punto de union.
//Repetir --checkpoint permite especificar mas de un checkpoint.
//Ejecucion objetivo: ./build/pr1 test/entorno01.txt salida.txt resultados.txt MODI3 --checkpoint 3,4
//Dos checkpoints: ./build/pr1 test/entorno01.txt salida.txt resultados.txt MODI3 --checkpoint 3,4 --checkpoint 6,2
//Ubicacion: Pr1/lib/AEstrella.h, junto a ResultadoBusqueda; vector ya esta incluido.
#if 0
struct OpcionesBusqueda {
    std::vector<Posicion> checkpoints;
};
#endif

//MODI[3]: En Pr1/lib/AEstrella.h, declarar una busqueda de tramo en private.
//El cuerpo de buscar debe extraerse a este metodo para permitir otro objetivo.
#if 0
ResultadoBusqueda buscarTramo(const Posicion& inicio,
                               const Posicion& objetivo,
                               bool pasoAPaso,
                               std::ostream* salida);
#endif

//MODI[3]: En buscarTramo (Pr1/src/AEstrella.cpp), adaptar el buscar existente.
//Usar inicio en vez de mapa.getOrigen(), objetivo en vez de mapa.getDestino(),
//y comparar posicionActual con objetivo para terminar el tramo.
#if 0
Nodo& nodoInicio = mapa.obtenerNodo(inicio);
nodoInicio.actualizarCostes(0, calcularHeuristica(inicio, objetivo), nullptr);
mejoresCostes[inicio] = 0;
abiertos.push(&nodoInicio);

// Dentro del bucle principal:
if (posicionActual == objetivo) {
    std::vector<Posicion> caminoTramo = reconstruirCamino(actual);
    return {caminoTramo, actual->getCosteAcumulado(), nodosGenerados,
            nodosInspeccionados, true};
}
#endif

//MODI[3]: En Pr1/src/AEstrella.cpp, envolver buscarTramo y buscar todos los tramos.
//La llamada al tramo siguiente comienza en el checkpoint anterior. Se omite el
//primer elemento de cada tramo posterior para no duplicar la coordenada compartida.
#if 0
std::vector<Posicion> objetivos = opciones.checkpoints;
objetivos.push_back(mapa.getDestino());

std::vector<Posicion> caminoCompleto;
Posicion inicio = mapa.getOrigen();
int costeTotal = 0;
for (const Posicion& objetivo : objetivos) {
    ResultadoBusqueda tramo = buscarTramo(inicio, objetivo, pasoAPaso, salida);
    if (!tramo.encontrado) {
        return {{}, 0, tramo.nodosGenerados,
                tramo.nodosInspeccionados, false};
    }

    const std::size_t primerIndice = caminoCompleto.empty() ? 0 : 1;
    caminoCompleto.insert(caminoCompleto.end(),
                          tramo.camino.begin() + primerIndice,
                          tramo.camino.end());
    costeTotal += tramo.coste;
    inicio = objetivo;
}
mapa.marcarCamino(caminoCompleto);
return {caminoCompleto, costeTotal, nodosGenerados,
        nodosInspeccionados, true};
#endif

//MODI[4]: Limitar el coste total de la ruta.
//Como funciona: se descartan sucesores cuyo coste acumulado supere el limite.
//Al haber checkpoints, cada tramo recibe el presupuesto que queda del total. Si
//no existe ruta que respete el limite, la busqueda devuelve que no hay camino.
//Ejecucion objetivo: ./build/pr1 test/entorno01.txt salida.txt resultados.txt MODI4 --max-cost 40
//Ubicacion: agregar costeMaximo a OpcionesBusqueda y pasarlo a buscarTramo.
//Usar -1 para indicar que no hay limite.
#if 0
struct OpcionesBusqueda {
    int costeMaximo = -1;
};
#endif

//MODI[4]: En la expansion de vecinos de buscarTramo, despues de calcular nuevoCoste.
//Para checkpoints, pasar al tramo el presupuesto restante (maximo - costeTotal).
#if 0
if (costeMaximo >= 0 && nuevoCoste > costeMaximo) {
    continue;
}
#endif

//MODI[4]: En el bucle de checkpoints, impedir empezar un tramo sin presupuesto.
#if 0
const int costeRestante = opciones.costeMaximo < 0
    ? -1
    : opciones.costeMaximo - costeTotal;
if (costeRestante < 0) {
    return {{}, 0, nodosGenerados, nodosInspeccionados, false};
}
ResultadoBusqueda tramo = buscarTramo(inicio, objetivo, costeRestante,
                                      pasoAPaso, salida);
#endif

//MODI[5]: Obstaculos dinamicos sin sobrescribir el coste original del terreno.
//Como funciona: se guardan los bloqueos temporales aparte del mapa. En modo paso
//a paso se aceptan los comandos block fila columna y unblock fila columna. Tras
//cada cambio se reinicia A* desde el origen para evitar reutilizar una frontera vieja.
//Ejecucion objetivo: ./build/pr1 test/entorno01.txt salida.txt resultados.txt MODI5 --step --dynamic-obstacles
//Cuando aparezca la pausa, se puede escribir, por ejemplo: block 4 5
//Ubicacion: Pr1/lib/Mapa.h, incluir <set>, agregar estos miembros y declarar setter.
#if 0
std::set<Posicion> obstaculosDinamicos;
void establecerObstaculoDinamico(const Posicion& posicion, bool bloquear);
#endif

//MODI[5]: En Pr1/src/Mapa.cpp, considerar el conjunto dinamico en esTransitable.
#if 0
bool Mapa::esTransitable(const Posicion& posicion) const {
    return esValida(posicion) &&
           nodos[posicion.first][posicion.second].esTransitable() &&
           obstaculosDinamicos.find(posicion) == obstaculosDinamicos.end();
}

void Mapa::establecerObstaculoDinamico(const Posicion& posicion,
                                       bool bloquear) {
    if (!esValida(posicion)) {
        throw std::out_of_range("Posicion fuera del mapa");
    }
    if (posicion == origen || posicion == destino) {
        throw std::invalid_argument("No se puede bloquear origen o destino");
    }
    if (bloquear) {
        obstaculosDinamicos.insert(posicion);
    } else {
        obstaculosDinamicos.erase(posicion);
    }
}
#endif

//MODI[5]: En el punto de pausa de modo paso a paso, leer block/unblock fila columna.
//Ubicacion: sustituir esperarPaso() en Pr1/src/AEstrella.cpp. Al cambiar el mapa,
//cancelar el tramo actual y reiniciar buscar desde el origen: asi no se usa una
//frontera calculada antes del cambio de obstaculo.
#if 0
std::string comando;
std::getline(std::cin, comando);
std::istringstream entrada(comando);
std::string accion;
int fila;
int columna;
if (entrada >> accion >> fila >> columna) {
    if (accion == "block") {
        mapa.establecerObstaculoDinamico({fila, columna}, true);
        reiniciarBusqueda = true;
    } else if (accion == "unblock") {
        mapa.establecerObstaculoDinamico({fila, columna}, false);
        reiniciarBusqueda = true;
    }
}
#endif

//MODI[1-5]: Para probar las opciones, agregarlas a los argumentos de main.cpp,
//conservando los valores por defecto para que la practica original siga funcionando.
//Ejemplos de activacion desde codigo:
#if 0
OpcionesBusqueda opciones;
opciones.permitirDiagonales = true;          //MODI[1]
opciones.checkpoints.push_back({3, 4});      //MODI[3]
opciones.costeMaximo = 40;                   //MODI[4]
opciones.obstaculosDinamicos = true;          //MODI[5]
ResultadoBusqueda resultado = algoritmo.buscar(pasoAPaso, &std::cout, opciones);
#endif
