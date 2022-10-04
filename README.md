# Algoritmo-genetico-de-pureza
Algoritmo que mediante la creación de hijos y la comparación con sus padres, se van generando generaciones con una "genética" más pura, empleando también un método de mutación genético.


El algoritmo llamado "AlgoritGenetico" se encarga de generar una sola generación extra, combinando el primer elemento con el último, el segundo con el penúltimo, así hasta que cada uno de los 10 elementos se hayan cruzado con otro. Comparando así, a los hijos con los padres para saber cuál es más puro, de esta manera se van guardando y reemplazando continuamente los hijos con los padres para ir obteniendo las generaciones más puras posibles.

El algoritmo llamado "AlgoritGenMasGen", hace el mismo proceso con la única diferencia que este algoritmo creará las generaciones que el usuario desee con un límite de 1000, la creación de más generaciones se detendrá si se cumple la condición de obtener un hijo con una genética completamente pura.

El tercer algoritmo, llamado "AlgoritGenMasGenRand", hace el mismo proceso que el anterior, pero este elige los padres de manera aleatoria, vigilando que ninguno de los 10 elementos se repitan en esa generación.

Por último, el cuarto algoritmo, llamado "AlgoritGenMasGenRand6P", hace lo mismo que el anterior, con la única diferencia que, en este algoritmo solo se tomarán a 6 elementos como padres en vez de los 10.
