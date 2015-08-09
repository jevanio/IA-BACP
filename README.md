# IA-BACP
Proyecto de IA, resolver BACP mediante algoritmo Greedy y Tabú Search

Instrucciones de uso:
1) En proyecto.c se indica la instancia, el largo de la lista tabú y la cantidad de iteraciones.
		#define Instancia "instancias/bacp8.txt"
		#define largo 50
		#define MAX_ITER 100000"
Para cambiar la instancia se debe cambiar la ruta del archivo.

2) Compilar y ejecutar en consola mediante el comando "make".
3) Se muestra en consola el listado de cursos indicando su carga académica, su periodo asignado, su lista de prerequisitos y de requisito.
4) Se genera en archivo "Resultado" en donde se muestran los cursos por periodo, la carga académica de cada periodo, la máxima carga académica y el tiempo de ejecución.
