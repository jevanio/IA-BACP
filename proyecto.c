#include <stdio.h>
#include <stdlib.h>

#define Instancia "bacp8.txt"

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
	struct _prereq *prereq;
	struct _req *req;
	struct _cursos *siguiente;
};

struct _malla *malla;
struct _cursos *primero, *ultimo;
int sbest[12];

/*
*	Lee la instancia y la ingresa a la representacion elegida: Lista Enlazada.
*/
void leer() {
	int i,flag;
	struct _cursos *nuevo, *aux;
	struct _prereq *prereq, *aux2;
	struct _req *req, *aux3;
	FILE *fp;

	/* 
	*	Información del curriculo
	*/
	malla = (struct _malla *) malloc (sizeof(struct _malla));
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
		nuevo->prereq=NULL;
		nuevo->req=NULL;
		nuevo->siguiente=NULL;

		if (primero==NULL) {
			primero = nuevo;
			ultimo = nuevo;
		}
		else {
			ultimo->siguiente = nuevo;
			ultimo = nuevo;
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
void mostrar(){
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
* Cuenta carga académica de cada periodo
*/
void cargar_cargaacademica_greedy() {
	int i;
	struct _cursos *aux;
	for(i=0;i<malla->n_periodos;i++) {
		sbest[i]=0;
		aux = primero;
		while(aux!=NULL) {
			if(aux->periodo==(i+1))
				sbest[i]+=aux->creditos;
			aux=aux->siguiente;
		}
	}
}

/*
*	Determina el periodo con menor carga académica. Si hay dos iguales se queda con el primero que encuentra.
*/
int min_periodo(int p) {
	int i, min_p=p-1,saux=sbest[0];
	for(i=p;i<malla->n_periodos;i++)
	{
		if(sbest[i]<saux) {
			saux=sbest[i];
			min_p=i;
		}
	}
	return min_p+1;
}

/*
* Indica si quedan cursos por asignar a un periodo, por defecto todos parten en 0.
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
*	Encontrar en lista cursos los que son prerequisitos y comprobar si están o no asignados
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
*	Mayor periodo de los prerequisitos
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
*	Algoritmo Greedy, construye la solución inicial.
*	Función de Evaluaciación: El periodo con mayor carga académia luego de minimizar la carga de todos los periodos.
*	Punto de Partida: Primer curso sin prerequisitos en primer periodo.
*	Función Miope:	Añadir curso sin prerequisitos en periodo con menor carga académica más cercano, busqueda ascendente. Si es requisito de otro no se puede asignar al ultimo periodo.
*					Cursos con prerequisitos se asignar solo al ser satisfechos. Esto es entre el periodo posterior 
*									a su prerequisito más cercano y el último periodo (salvo sean requisito de otro).
*/
void greedy() {
	struct _cursos *aux;
	aux=primero;
	/*
	*	Elegir punto de partida.
	*/
	while(aux!=NULL) {
		if(aux->prereq==NULL) {
			aux->periodo=1;
			if (aux==ultimo)
				aux=primero;
			else
				aux=aux->siguiente;
			break;
		}
		aux=aux->siguiente;
	}
	/*
	*	Bucle hasta asignar todos a un periodo.
	*/
	while(porasignar()) {
		cargar_cargaacademica_greedy();
		while(prerequisitos_insatisfechos(aux->prereq) || aux->periodo!=0) {
			if (aux==ultimo)
				aux=primero;
			else
				aux=aux->siguiente;
		}
		if(aux->prereq==NULL) {
			/*
			*	Asignar al periodo con menor cantidad de carga.
			*/
			aux->periodo=min_periodo(1);
		}
		else {
			// Encontrar el periodo más lejano de sus prerequisitos, asignar entre ese y el infinito al perido con menor carga academica.
			aux->periodo=min_periodo(ultimo_periodo(aux->prereq));
		}
	}
}

/*
*	Archivo Solución
*/
void archivo_solucion() {

}
/*
*	Iniciando del todo
*/
int main() {
	primero = (struct _cursos *) NULL;
	ultimo = (struct _cursos *) NULL;
	leer();	
	greedy();
	cargar_cargaacademica_greedy();
	//mostrar();

	return 0;
 }