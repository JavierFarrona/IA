# Práctica 1: búsqueda A*

La entrada es una matriz rectangular de enteros separada por espacios. `0` es
el origen, `10` el destino y `-1` un obstáculo; cualquier otro valor positivo
es el coste de entrar en esa celda. La entrada al destino cuesta siempre `2`.

La búsqueda usa únicamente movimientos ortogonales y `f(n) = g(n) + h(n)`,
con `h(n) = 2 * distancia Manhattan`. En la salida del mapa, las celdas del
camino se representan únicamente con `*`; las demás conservan su valor
original.

Uso:

```text
make
build/pr1 test/entorno01.txt mapa-salida.txt resultados.txt ID1
```

El resultado se muestra por pantalla y se guarda en el fichero indicado. La
información se escribe en líneas separadas: primero `Instancia n m co cd`, y
después `Camino`, `Coste`, `Nodos generados` y `Nodos inspeccionados`. En esta
implementación `co` y `cd` son las coordenadas explícitas del origen y del
destino, con formato `co: (fila,columna) cd: (fila,columna)`. Ambos últimos
campos contienen coordenadas con formato `(fila,columna)`, sin
duplicados y en orden determinista. `Nodos generados` es el conjunto actual de
nodos generados que siguen pendientes de inspección; `Nodos inspeccionados`
acumula los nodos extraídos y procesados. Por tanto, al inspeccionar un nodo se
elimina de `Nodos generados` y se añade a `Nodos inspeccionados`.