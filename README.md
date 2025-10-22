***
>***Universidad de La Laguna***
>***Escuela Superior de Informática y Tecnología***
>**Asignatura:** Inteligencia Artificial
>**Autor:** Francisco R. Fdez Gómez
>**Contacto:** alu0101427324@ull.edu.es

# Diseño e Implementación de la Búsqueda A*

## 1. Introducción

>El algoritmo A* es una técnica de búsqueda informada utilizada para encontrar el camino óptimo entre dos puntos en un grafo, aplicándose aquí sobre una cuadrícula que representa el entorno. El proyecto está implementado en C++ y consta de una organización modular, con clases específicas para cada elemento fundamental del proceso de búsqueda y navegación.

## 2. Componentes principales y sus responsabilidades

### 2.1. `Grid` (Entorno)

- **Propósito:** Representa el espacio dónde opera el algoritmo, modelado como una matriz bidimensional que simula una cuadrícula de celdas.
- **Atributos:**  
  - `grid_`: vector 2D que contiene el tipo de cada celda (`CellType`).
  - `rows_`, `columns_`: dimensiones de la cuadrícula.
  - `start_`, `goal_`: celdas de inicio y objetivo.
  - `cell_count_`, `wall_count_`: métricas sobre el entorno, usadas para estadísticas.
- **Funciones clave:**  
  - `GetCellType(x, y)`: Devuelve el tipo de celda en la posición dada.
  - `GetNeighbours(cell)`: Calcula vecinos accesibles y la acción para alcanzarlos, considerando muros y límites.
  - `UpdateGrid(p_in, p_out)`: Permite la modificación probabilística de la cuadrícula.
  - `CalculateWallsPercentage()`: Cuantifica la densidad de muros.

### 2.2. `Cell` (Celda)

- **Propósito:** Modela una celda individual de la cuadrícula, con su posición (x, y) y tipo (`CellType`).
- **Funciones auxiliares:**  
  - Operadores de comparación (`==`, `!=`).
  - Especialización de hash para uso en contenedores desordenados (`unordered_set`).

### 2.3. `Node` (Nodo de Búsqueda)

- **Propósito:** Representa un estado dentro del árbol de búsqueda.
- **Atributos:**  
  - `fatherptr_`: Puntero al nodo padre (permite reconstruir el camino).
  - `cost_g_`: Coste real desde el origen hasta este nodo.
  - `cost_h_`: Estimación heurística desde este nodo a la meta.
  - `cost_f_`: Suma total (`cost_f_ = cost_g_ + cost_h_`).
  - `cell_`: Celda asociada a este nodo.
- **Funciones clave:**  
  - Getters/Setters para cada atributo, permitiendo modificación en tiempo de ejecución.

### 2.4. Enumeraciones

- **`CellType`:** Identifica los tipos de celda posibles (vacía, muro, inicio, objetivo, agente, camino, recorrido).
- **`Action`:** Modela los movimientos permitidos (N, S, E, O y diagonales).

### 2.5. `Agent` (Agente)

- **Propósito:** Simula un agente que recorre el entorno siguiendo el camino hallado.
- **Atributos:**  
  - `position_`: Celda actual del agente.
  - `environment_`: Referencia a la cuadrícula por la que se mueve.
  - `path_`: Registro del trayecto realizado.
- **Funciones clave:**  
  - `Step(path)`: Mueve el agente a lo largo de los nodos definidos por el camino A*.
### 2.6. `Metrics` (Métricas del Algoritmo)

- **Propósito:** Registrar estadísticas relevantes del proceso de búsqueda, que permiten analizar el rendimiento y comportamiento del algoritmo.
- **Atributos principales:**  
  - `generated_nodes`: Contador de nodos generados durante la búsqueda.
  - `inspected_nodes`: Contador de nodos inspeccionados (cerrados).
  - `cost_of_path_walked`: Coste total del camino encontrado.
  - `path`: Secuencia de celdas que conforman el camino óptimo.
- **Funciones y usos:**  
  - Registro de métricas como parte de la ejecución de `SearchAStar::Run`.
  - Impresión y visualización de los resultados en tablas y estadísticas al finalizar la búsqueda (por ejemplo, con `PrintResultsTable()`).
  - Facilita la comparación entre instancias, ajustes de heurística o cambios de configuración del grid.

## 3. Algoritmo A* y estructuras de datos específicas

### 3.1. Estructuras clave para la búsqueda

- **`open_list_` (`std::unordered_set<Node*>`):** Contiene los nodos frontera (a explotar), seleccionando siempre el de menor coste total `f`.
- **`closed_list_` (`std::unordered_set<Cell>`):** Almacena las celdas ya inspeccionadas para evitar re-exploración y ciclos.
- **Vectores auxiliares:** Copias de los sets principales para facilitar visualización y depuración.

### 3.2. Flujo del Algoritmo

1. **Inicialización**  
   - Se calcula `g`, `h` y `f` en el nodo inicial, añadiéndolo a la lista de abiertos.
   - Se inicializan contadores de métricas (nodos generados, inspeccionados, coste recorrido).

2. **Bucle Principal**
   - Mientras haya nodos en la lista abierta:
     - Escoge el nodo con menor `f`.
     - Si es la meta, reconstruye el camino utilizando los punteros al padre.
     - Si no, para cada vecino:
       - Si no está en abiertos/cerrados, se añade como nuevo nodo.
       - Si ya está en abiertos y el nuevo camino propuesto es mejor, se actualiza el nodo padre y los costes.
     - Marca el nodo actual como cerrado.
   - Si se agotan las alternativas y no se alcanza la meta, informa que no existe camino.

3. **Funciones auxiliares**
   - `ManhattanDistance(node, end, weight)`: Calcula la heurística principal.
   - `SearchNodeWithLowestF(open_list)`: Retorna el nodo abierto de menor coste total.
   - `IsInOpenList(node, open_list)`: Comprueba si una celda está en la frontera.
   - `WhoIsBetterNode(node1)`: Compara dos nodos y actualiza el que sea óptimo.

4. **Reconstrucción y Resultados**
   - El método `ReconstructPath(Node*)` permite recuperar el camino desde el objetivo hasta el inicio siguiendo los nodos padre.
   - El resultado y camino se presenta junto al coste, métricas y sucesión de acciones.

## 4. Diseño modular y extensibilidad

>- Cada componente está separado en archivos individuales, siguiendo buenas prácticas de encapsulamiento.
>- El código permite modificar la heurística, añadir otros movimientos o condiciones de grid de manera sencilla.
>- La estructura permite incorporar futuros agentes, métricas adicionales o variantes del entorno sin romper la lógica del algoritmo central.

## 5. Ejecución y Consideraciones

>- El flujo principal reside en la clase `SearchAStar`. El entorno (`Grid`), el nodo de partida (`Cell`), el agente (`Agent`) y las métricas (`Metrics`) se orquestan para ejecutar el algoritmo, obtener el resultado óptimo y visualizar el proceso de búsqueda.
>- Se puede personalizar el grid (punto de inicio y fin), peso de la heurística y probabilidades de actualización del grid.

---
