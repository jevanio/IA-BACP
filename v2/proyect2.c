#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define Instancia "../instancias/bacp8.txt"
#define largo 5

struct _malla{
	int n_cursos;
	int n_periodos;
	int min_creditos;
	int max_creditos;
	int min_cursos;
	int max_cursos;
	int n_prerequisitos;
};

struct _prereq {
	int curso;
	struct _prereq *siguiente;
};

struct _req {
	int curso;
	struct _req *siguiente;
};

struct _cursos {
	int pos;
	int creditos;
	int periodo;
	int cadena;
	struct _prereq *prereq;
	struct _req *req;
	struct _cursos *siguiente;
};

struct _LS {
	int curso;
	int periodo;
	struct _LS *siguiente;
};

/*
*	encontrar ultimo elemento de la lista
*/
struct _cursos *getultimo(struct _cursos *aux) {
	while(aux->siguiente!=NULL)
		aux=aux->siguiente;
	return aux;
}

/*
*	Lee la instancia y la ingresa a la representacion elegida: Lista Enlazada.
*/
void leer(struct _malla **mallas, struct _cursos **curso) {
	int i,flag;
	struct _cursos *nuevo, *aux;
	struct _prereq *prereq, *aux2;
	struct _req *req, *aux3;
	struct _malla *malla;
	FILE *fp;
	malla = *mallas;
	nuevo = *curso;
	/* 
	*	Información del curriculo
	*/
	/*if (nuevo==NULL) printf( "No hay memoria disponible!\n");*/

	fp = fopen (Instancia, "r");
	/*if (fp==NULL) {fputs ("File error",stderr); exit(1);}*/
	fscanf(fp, "%d %d %d %d %d %d %d \n" ,&malla->n_cursos, &malla->n_periodos, &malla->min_creditos, &malla->max_creditos, &malla->min_cursos, &malla->max_cursos, &malla->n_prerequisitos);

	/*
	*	Información de cada curso
	*/
	for(i=0;i<malla->n_cursos;i++) {
		nuevo = (struct _cursos *) malloc (sizeof(struct _cursos));
		nuevo->pos = i;
		fscanf(fp, "%d ", &nuevo->creditos);
		nuevo->periodo=0;
		nuevo->cadena=0;
		nuevo->prereq=NULL;
		nuevo->req=NULL;
		nuevo->siguiente=NULL;

		if (primero==NULL) {
			primero = nuevo;
		}
		else {
			getultimo(primero)->siguiente = nuevo;
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
*	Muestra la información recolectada de los cursos.
*/
void mostrar(struct _malla *malla, struct _cursos *aux){
	struct _prereq *aux2;
	struct _req *aux3;

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
		printf("%d\n", aux->cadena);
		aux=aux->siguiente;
	}
	printf("\n Cursos: %d\n Periodos: %d\n Min_Creditos: %d\n Max_Creditos: %d\n Min_Cursos: %d\n Max_Cursos: %d\n Prerequisitos: %d\n", malla->n_cursos, malla->n_periodos, malla->min_creditos, malla->max_creditos, malla->min_cursos, malla->max_cursos, malla->n_prerequisitos);
}

/*
*	Determina el periodo con menor carga académica. Si hay dos iguales se queda con el primero que encuentra.
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


int cumple_min(struct _malla *malla, int carga_periodo[], int cursos_periodo[], int ult) {
	int i;
	for(i=ult;i<malla->n_periodos;i++) {
		if(carga_periodo[i]<malla->min_creditos || cursos_periodo[i]<malla->min_cursos)
			return i+1;
	}
	return 0;
}

int cumple_max(struct _malla *malla, int carga_periodo[], int cursos_periodo[], int creditos, int ult) {
	int i;
	for (i=ult;i<malla->n_periodos;i++) {
		if((carga_periodo[i]+creditos)<=malla->max_creditos && cursos_periodo[i]<malla->max_cursos)
			return i+1;
	}
	return 0;
}

/*
*	Ver el largo de lista tabu
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
*	Archivo Solución
*/
void archivo_solucion(struct _malla *malla, struct _cursos *aux, clock_t t1, int carga_periodo[]) {
	int i;
	FILE *fp;
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
*	Iniciando del todo
*/
int main() {
	clock_t t1=clock();
	struct _malla *malla;
	struct _cursos *primero;
	int carga_periodo[]={0,0,0,0,0,0,0,0,0,0,0,0};
	int cursos_periodo[]={0,0,0,0,0,0,0,0,0,0,0,0};
	malla = (struct _malla *) malloc (sizeof(struct _malla));
	primero = (struct _cursos *) NULL;

	/* Leer archivo de instancia */
	leer(&malla,&primero);
	mostrar(malla,primero);
	/* Medir cadena de dependencia */
//	cadena(primero);
	/* Ordenar por largo de cadena de dependencia */
//	primero = quickSortRecur(primero, getultimo(primero),2);
	/* greedy */
//	greedy(malla,carga_periodo,cursos_periodo);
//	tabusearch(malla,carga_periodo,cursos_periodo);
	/* Ordenar por periodo */
	//primero = quickSortRecur(primero, getultimo(primero),1);
	/* Crear archivo con el resultado */
	//archivo_solucion(malla, primero, t1,carga_periodo);
	return 0;
 }