# Documento de reporte
## _goldbach_optimization_
## Adrián Rojas - B96789

Antes de realizar las optimizaciones relacionadas con el mapeo de los hilos, fue necesario optimizar la versión serial para reducir el tiempo de ejecución, con el fin de no tardar tanto realizando las corridas para las pruebas de profiling. Para esto se buscó reducir el número de ciclos realizados a la hora de buscar los números que conforman la suma. La primera versión realizaba una gran cantidad de iteraciones innecesarias, con lo cual se podían eliminar reduciendo el número de iteraciones a más de la mitad.

<img src="../../src/Kcachegrind.png">

Tras analizar las estadisticas proporcionadas por la herramienta de profiling Kcachegrind, se determinó que una de las funciones especificas que más demada procesamiento es la funcion sqrt de la libreria math.h. esta funcion se llamaba en check_prime, la cual era una de las funciones mas utilizadas en find_sums, la que a su vez era de las regiones de procesamiento mas demandante en el codigo. El diseño de optimización busca disminuir esta demanda al sustituir la funcion sqrt por un ciclo con menos iteraciones. Ademas se incluyó una mascara para facilitar la deteccion de los numeros primos mas comunes en las sumas, que se encuentran desde el 3 hasta el 61. 

Tras implementar el diseño de optimización se consiguó reducir el tiempo de 245 segundos a 119 segundos. Este incremento se evidencia en el speedup, el cual pasó de 3.13 a 6.41.

## Comparacion de optimizaciones
<img src="../../src/Optimizations.png">

A pesar de que la version serial tenia una duracion muy elevada, las optimizaciones lograron reducir este tiempo considerablemente. La optimización que tuvo el menor impacto fue el mapeo por cilcos. Aunque este mapeo es le mas facil de implementar, su speedup fue realtivamente bajo. El mapeo por bloque prodría tener una mejora mas destacable con respecto a la duración. Las siguientes optimizaciónes sí lograron un mayor impacto en el rendimiento, reduciendo la duración a la mitad con respecto a su predecesor. La ultima optimizacion en la función check_prime fue la de mayor eficiencia entre las tres. 

<img src="../../src/Concurrency_level.png">

En el ultimo grafico se puede apreciar la relacion entre el grado de concurrencia y los tiempos conseguidos. Se puede determinar que entre mas hilos se consigué una menor duración, pero esto tambien le cuesta recursos a la computadora, por lo que se vuelve ineficiente asignarle tantos hilos al programa. Especificamente a partir de los 8 hilos, el speedup es muy similar al de los siguientes grados de concurrencia, y esto se puede ver reflejado en los tiempos de ejecución. Por lo tanto, la cantidad de hilos optima para conseguir el mejor rendimiento sería de 8 hilos, debido a la relación tiempo-eficiencia.
