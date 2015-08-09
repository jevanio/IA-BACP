#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sort.h"
#define Instancia "instancias/bacp8.txt"
#define largo 50
#define MAX_ITER 100000

struct _malla{
	int n_cursos;
	int n_periodos;
	int min_creditos;
	int max_creditos;
	int min_cursos;
	int max_cursos;
	int n_prerequisitos;
};

struct _LS {
	int curso;
	int periodo;
	struct _LS *siguiente;
};

struct _cursos *primero;

/*
*	Lee la instancia y la ingresa a la representacion elegida: Lista Enlazada.
*/
void leer(struct _malla **mallas) {
	int i,flag;
	struct _cursos *nuevo, *aux;
	struct _prereq *prereq, *aux2;
	struct _req *req, *aux3;
	struct _malla *malla;
	FILE *fp;
	malla = *mallas;

	/* 
	*	Información de la malla
	*/
	fp = fopen (Instancia, "r");
	fscanf(fp, "%d %d %d %d %d %d %d \n" ,&malla->n_cursos, &malla->n_periodos, &malla->min_creditos, &malla->max_creditos, &malla->min_cursos, &malla->max_cursos, &malla->n_prerequisitos);

	/*
	*	Información de cada curso
	*/
	for(i=0;i<malla->n_cursos;i++) {
		nuevo = (struct _cursos *) malloc (sizeof(struct _cursos));
		nuevo->pos = i;
		fscanf(fp, "%d ", &nuevo->creditos);
		nuevo->periodo=0;
		nuevo->grado=0;
		nuevo->prereq=NULL;
		nuevo->req=NULL;
		nuevo->siguiente=NULL;

		if (primero==NULL) {
			primero = nuevo;
		}
		else {
			get_last(primero)->siguiente = nuevo;
		}
	}

	/*
	*	Añadir Prerequisitos a cada curso
	*/
	for(i=0;i<malla->n_prerequisitos;i++) {
		aux=primero;
		fscanf(fp,"%d",&flag);

		while(flag != aux->pos) {
			aux=aux->siguiente;
		}

		prereq = (struct _prereq *) malloc (sizeof(struct _prereq));
		fscanf(fp,"%d", &prereq->curso);
		prereq->siguiente=NULL;
		req = (struct _req *) malloc (sizeof(struct _req));
		req->curso=flag;
		req->siguiente=NULL;

		if(aux->prereq==NULL) {
			aux->prereq	= prereq;
		}
		else {
			aux2=aux->prereq;
			while(aux2->siguiente!=NULL) {
				aux2=aux2->siguiente;
			}
			aux2->siguiente=prereq;
		}

		aux=primero;
		while(aux->pos != prereq->curso) {
			aux=aux->siguiente;
		}
		if(aux->req==NULL) {
			aux->req=req;
		}
		else{
			aux3=aux->req;
			while(aux3->siguiente!=NULL) {
				aux3=aux3->siguiente;
			}
			aux3->siguiente=req;
		}
	}
	fclose (fp);
}

/*
*	Muestra la información de la instancia (de cursos y de la malla).
*/
void mostrar(struct _malla *malla){
	struct _cursos *aux;
	struct _prereq *aux2;
	struct _req *aux3;

	aux = primero;
	while(aux!=NULL) {
		printf("\n Curso: %d\n Créditos: %d\n Periodo: %d\n", aux->pos, aux->creditos, aux->periodo);
		if(aux->prereq==NULL) {
			printf("No existen Prerequisitos\n");
		}
		else {
			printf("Prerequisitos:");
			aux2=aux->prereq;
			while(aux2!=NULL) {
				printf(" %d", aux2->curso);
				aux2=aux2->siguiente;
			}
			printf("\n");
		}

		if(aux->req==NULL) {
			printf("No es Requisito\n");
		}
		else {
			printf("Requisito de:");
			aux3=aux->req;
			while(aux3!=NULL) {
				printf(" %d", aux3->curso);
				aux3=aux3->siguiente;
			}
			printf("\n");
		}
		aux=aux->siguiente;
	}
	printf("\n Cursos: %d\n Periodos: %d\n Min_Creditos: %d\n Max_Creditos: %d\n Min_Cursos: %d\n Max_Cursos: %d\n Prerequisitos: %d\n", malla->n_cursos, malla->n_periodos, malla->min_creditos, malla->max_creditos, malla->min_cursos, malla->max_cursos, malla->n_prerequisitos);
}

/*
*	Determina la maxima carga académica.
*/
int max_periodo(struct _malla *malla,int carga_periodo[]) {
	int i, saux=carga_periodo[0];
	for(i=0;i<malla->n_periodos;i++)
	{
		if(carga_periodo[i]>saux) {
			saux=carga_periodo[i];
		}
	}
	return saux;
}

/*
* Indica si quedan cursos por asignar a un periodo, para ello verifica si el periodo es 0 (Valor por defecto).
*/
int porasignar() {
	struct _cursos *aux;
	aux=primero;
	while(aux!=NULL) {
		if(aux->periodo==0)
			return 1;
		aux=aux->siguiente;
	}
	return 0;
}

/*
*	Según la lista de prerequisitos, se busca algún curso que tenga todos sus prerequisitos asignados.
*/
int prerequisitos_insatisfechos(struct _prereq *aux2) {
	struct _cursos *aux;
	while(aux2!=NULL) {
		aux=primero;
		while(aux!=NULL) {
			if (aux->pos==aux2->curso && aux->periodo==0)
				return 1;
			aux=aux->siguiente;
		}
		aux2=aux2->siguiente;
	}
	return 0;
}

/*
*	A partir de la lista de prerequisitos de un curso, busca el periodo más tardio al que se asignaron.
*/

int ultimo_periodo(struct _prereq *aux2) {
	struct _cursos *aux;
	int ult_per=0;
	int temp_per;
	while(aux2!=NULL) {
		aux=primero;
		while(aux!=NULL) {
			if (aux->pos==aux2->curso){
				temp_per=aux->periodo;
				break;
			}
			aux=aux->siguiente;
		}
		if (temp_per>ult_per)
			ult_per=temp_per;
		aux2=aux2->siguiente;
	}
	return ult_per;
}

/*
* Comprueba que se cumpla el mínimo de carga académica y de cursos por periodo.
*/
int cumple_min(struct _malla *malla, int carga_periodo[], int cursos_periodo[], int ult) {
	int i;
	for(i=ult;i<malla->n_periodos;i++) {
		if(carga_periodo[i]<malla->min_creditos || cursos_periodo[i]<malla->min_cursos)
			return i+1;
	}
	return 0;
}

/*
* Comprueba que mp se incumpla el max de carga académica y de cursos por periodo.
*/
int cumple_max(struct _malla *malla, int carga_periodo[], int cursos_periodo[], int creditos, int ult) {
	int i;
	for (i=ult;i<malla->n_periodos;i++) {
		if((carga_periodo[i]+creditos)<=malla->max_creditos && cursos_periodo[i]<malla->max_cursos)
			return i+1;
	}
	return 0;
}

/*
*	Algoritmo Greedy, construye la solución inicial en base a la lista previamente ordenada de modo descendente respecto al grado de dependencia.
*	Inicia asignando al primer periodo el primer curso que no tenga prerequisitos.
*	Luego, se asigna los demás cursos sin asignar, siempre y cuando sus prerequisitos estén satisfechos. Para hacerlo hay dos criterios:
*		1) Si algún periodo incumple sus mínimos se asigna a dicho periodo siempre y cuando sea un periodo posterior a sus prerequisitos.
*		2) Si todos los periodos cumplen sus mínimos, se asigna al primer periodo donde no incumpla los máximo y sea posterior a sus prerequisitos.
*/
void greedy(struct _malla *malla, int carga_periodo[], int cursos_periodo[]) {
	struct _cursos *aux;
	aux=primero;
	int ult, min, max;
	/*
	*	Elegir punto de partida.
	*/
	while(aux!=NULL) {
		if(aux->prereq==NULL) {
			aux->periodo=1;
			carga_periodo[0]=aux->creditos;
			cursos_periodo[0]=1;
			break;
		}
		aux=aux->siguiente;
	}
	/*
	*	Bucle hasta asignar todos a un periodo.
	*/
	while(porasignar()) {
		while(prerequisitos_insatisfechos(aux->prereq) || aux->periodo!=0) {
			if (aux==get_last(primero))
				aux=primero;
			else
				aux=aux->siguiente;
		}
		ult=ultimo_periodo(aux->prereq);
		min=cumple_min(malla, carga_periodo, cursos_periodo, ult);
		if(min) {
			aux->periodo=min;
			carga_periodo[aux->periodo-1]+=aux->creditos;
			cursos_periodo[aux->periodo-1]+=1;
		}
		else {
			max=cumple_max(malla, carga_periodo, cursos_periodo, aux->creditos, ult);
			aux->periodo=max;
			carga_periodo[aux->periodo-1]+=aux->creditos;
			cursos_periodo[aux->periodo-1]+=1;
		}
		aux=primero;
	}
}

/*
*	Ver largo de lista tabu
*/
int lenTS(struct _LS *ls){
	int i=0;
	while(ls!=NULL) {
		i+=1;
		ls=ls->siguiente;
	}
	return i;
}

/*
*	Comprueba que el miembro del vecindario cumpla los prerequisitos.
*/
int cumple_prereq(struct _prereq *prereq, int nuevo_periodo) {
	struct _cursos *aux;
	while(prereq!=NULL) {
		aux=primero;
		while(aux!=NULL) {
			
			if(aux->pos==prereq->curso && aux->periodo>=nuevo_periodo)
				return 1;
			aux=aux->siguiente;
		}
		prereq=prereq->siguiente;
	}
	return 0;
}

/*
*	Comprueba que el miembro del vecindario cumpla los requisitos.
*/
int cumple_req(struct _req *req, int nuevo_periodo) {
	struct _cursos *aux;
	while(req!=NULL) {
		aux=primero;
		while(aux!=NULL) {
			
			if(aux->pos==req->curso && aux->periodo<=nuevo_periodo)
				return 1;
			aux=aux->siguiente;
		}
		req=req->siguiente;
	}
	return 0;
}

/*
*	Inserta al inicio de la lista
*/
void push(int curso, int periodo, struct _LS **LS) {
	struct _LS *nuevo;
	nuevo = (struct _LS *) malloc (sizeof(struct _LS));
	nuevo->curso=curso;
	nuevo->periodo=periodo;
	nuevo->siguiente=*LS;
	*LS = nuevo;
}

/*
*	Elimina el último elemento de la lista
*/
void delete(struct _LS **LS) {
	struct _LS *aux=*LS;
	struct _LS *temp;
	while(aux->siguiente!=NULL) {
		temp=aux;
		aux=aux->siguiente;
	}
	free(temp->siguiente);
	temp->siguiente=NULL;
}

/*
*	Chequea si la solución candidata es factible, en caso de no serlo no lo admite en el vecindario.
*/
int infactible(struct _malla *malla, int carga_periodo[], int cursos_periodo[], int creditos, struct _cursos *aux, int nuevo_periodo) {
	int ult=0;
	struct _prereq *prereq=aux->prereq;
	struct _req *req=aux->req;
	if (!cumple_max(malla, carga_periodo, cursos_periodo, creditos, ult) || cumple_min(malla, carga_periodo, cursos_periodo, ult) ||
		cumple_prereq(prereq,nuevo_periodo) || cumple_req(req,nuevo_periodo))
		return 1;
	return 0;
}

/*
*	Revisa si el movimiento existe en la lista tabú, de ser así lo impide
*/
int check_tabulist(struct _LS *LS, int curso, int periodo) {
	while(LS!=NULL) {
		if(LS->curso==curso && LS->periodo==periodo)
			return 0;
		LS=LS->siguiente;
	}
	return 1;
}

/*
*	Tabu Search.
*	En cada iteración se cambia de periodo un curso y se verifica:
*		1) ¿El movimiento existe en la lista tabú?
*		2) ¿La solución candidata es infactible?
*		3) ¿La solución canditada es peor que la actual?
*	Si alguno de estos es afirmativo entonces se rechaza la solución y el curso se cambia a otro periodo.
*	En cambio de no poder cambiar a otro periodo el curso, se regresa al original y se cambia de periodo el siguiente curso de la lista.
*/
void tabusearch(struct _malla *malla, struct _cursos *s_actual, struct _LS **LS, int carga_periodo[], int cursos_periodo[]) {
	struct _cursos *aux;
	int carga_periodo_temp[12], cursos_periodo_temp[12];
	int i,j,pos,periodo,creditos,cambio,iteraciones=0;
	for(j=0;j<malla->n_periodos;j++) {
		carga_periodo_temp[j]=carga_periodo[j];
		cursos_periodo_temp[j]=cursos_periodo[j];
	}

	do {
		cambio=0;
		s_actual=primero;
		while(s_actual!=NULL) {
			pos=s_actual->pos;
			periodo=s_actual->periodo;
			creditos=s_actual->creditos;
			for(i=0;i<malla->n_periodos;i++) {
				if(i!=(periodo-1) && check_tabulist(*LS,pos,i+1)) {
					carga_periodo_temp[periodo-1]-=creditos;
					cursos_periodo_temp[periodo-1]-=1;
					carga_periodo_temp[i]+=creditos;
					cursos_periodo_temp[i]+=1;
					if(max_periodo(malla,carga_periodo)>=max_periodo(malla,carga_periodo_temp) &&
					 !infactible(malla, carga_periodo_temp, cursos_periodo_temp, creditos,s_actual,i+1)) {
						
						for(j=0;j<malla->n_periodos;j++) {
							carga_periodo[j]=carga_periodo_temp[j];
							cursos_periodo[j]=cursos_periodo_temp[j];
						}
						aux=primero;
						while(aux!=NULL) {
							if(aux->pos==pos){
								s_actual->periodo=i+1;
								cambio=1;
								push(pos,periodo,&(*LS));
								if(lenTS(*LS)>largo)
									delete(&(*LS));
								break;
							}
							aux=aux->siguiente;
						}
						break;
					}
					else {
						for(j=0;j<malla->n_periodos;j++) {
							carga_periodo_temp[j]=carga_periodo[j];
							cursos_periodo_temp[j]=cursos_periodo[j];
						}
					}
				}
			}
			if (cambio)
				break;
			else
				s_actual=s_actual->siguiente;
		}
		iteraciones+=1;
	}while(cambio && iteraciones<MAX_ITER);
}

/*
*	Da el alto de un árbol, esto permite conocer el grado de dependencia de cada curso.
*/
int altura(struct _cursos *curso){
	int max=0,temp;
	struct _req *req=curso->req;
	struct _cursos *aux;
	if (req==NULL)
		return 0;
	else {
		while(req!=NULL) {
			aux=primero;
			while(aux!=NULL) {
				if(aux->pos==req->curso) {
					temp=altura(aux);
					if(temp>max)
						max=temp;
					break;
				}
				aux=aux->siguiente;
			}
			req=req->siguiente;
		}
		return max+1;
	}
}

/*
* Mide la altura de cada arbol de restricciones
*/
void grado(struct _cursos *aux) {
	while(aux!=NULL) {
		aux->grado=altura(aux);
		aux=aux->siguiente;
	}
}

/*
*	Genera el archivo Solución
*/
void archivo_solucion(struct _malla *malla, clock_t t1, int carga_periodo[]) {
	struct _cursos *aux;
	int i;
	FILE *fp;

	aux=primero;
	fp = fopen ("Resultado", "w");
	fprintf(fp, "Salida Instancia %s\n", Instancia);
	for(i=0;i<malla->n_periodos;i++) {
		fprintf(fp, "Periodo %d\n", i+1);
		fprintf(fp, "%d ", aux->pos);
		aux=aux->siguiente;
		if(aux==NULL) {
			fprintf(fp, ": %d\n", carga_periodo[i]);
			break;
		}
		while(aux->periodo==(i+1)) {
			fprintf(fp, "- ");
			fprintf(fp, "%d ", aux->pos);
			aux=aux->siguiente;
			if(aux==NULL)
				break;
		}
		fprintf(fp, ": %d\n", carga_periodo[i]);
	}
	fprintf(fp, "\n");
	fprintf(fp, "Máxima Carga Académica %d\n",max_periodo(malla,carga_periodo));
	fprintf(fp, "Tiempo Ejecución %f s", ((double)clock() - t1) / (double)CLOCKS_PER_SEC);
	fclose(fp);
}

/*
*	Inicio y Final del todo.
*/
int main() {
	clock_t t1=clock();
	struct _malla *malla;
	struct _LS *LS;
	int carga_periodo[]={0,0,0,0,0,0,0,0,0,0,0,0};
	int cursos_periodo[]={0,0,0,0,0,0,0,0,0,0,0,0};
	malla = (struct _malla *) malloc (sizeof(struct _malla));
	primero = (struct _cursos *) NULL;
	LS = (struct _LS *) NULL;

	/* Leer archivo de instancia */
	leer(&malla);
	/* Medir grado de dependencia */
	grado(primero);
	/* Orden descendente por grado de dependencia */
	primero = quickSortRecur(primero, get_last(primero),1);
	
	/* greedy */
	greedy(malla,carga_periodo,cursos_periodo);

	/* Orden descendente por grado de dependencia */
	//primero = quickSortRecur(primero, get_last(primero),1);
	/* Orden ascendente por grado de dependencia */
	//primero = quickSortRecur(primero, get_last(primero),2);
	/* Orden descendente por periodo */
	//primero = quickSortRecur(primero, get_last(primero),3);
	/* Orden ascendente por periodo */
	//primero = quickSortRecur(primero, get_last(primero),4);
	
	/* Da un orden aleatorio a la lista de cursos */
	shuffle_sort(&primero);
	
	/* TabuSearch */
	tabusearch(malla,primero,&LS,carga_periodo,cursos_periodo);
	
	/* Ordenar ascendente por periodo */
	primero = quickSortRecur(primero, get_last(primero),4);
	mostrar(malla);
	/* Crear archivo con el resultado */
	archivo_solucion(malla, t1,carga_periodo);
	return 0;
 }