# IA-BACP
Proyecto de IA, resolver BACP mediante algoritmo Greedy y Tabú Search

Instrucciones de uso:
1) En proyecto.c se indica la instancia, el largo de la lista tabú y la cantidad de iteraciones.
		#define Instancia "bacp8.txt"
		#define largo 50
		#define MAX_ITER 100000"
2) Compilar y ejecutar en consola mediante el comando "make".
3) Se genera en archivo "Resultado" en donde se muestran los cursos por periodo, la carga académica de cada periodo, la máxima carga académica y el tiempo de ejecución.
4) Para cambiar el orden de selección se debe comentar/descomentar las líneas 581 (Grado de Dependencia), 583 (Periodo) y 586 (Aleatorio).
