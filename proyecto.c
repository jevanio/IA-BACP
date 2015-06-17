#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define Instancia "instancias/bacp12.txt"

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

struct _cursos *primero;
int sbest[12];

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
void leer(struct _malla **mallas) {
	int i,flag;
	struct _cursos *nuevo, *aux;
	struct _prereq *prereq, *aux2;
	struct _req *req, *aux3;
	struct _malla *malla;
	FILE *fp;
	malla = *mallas;

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
* Cuenta carga académica de cada periodo
*/
void cargar_cargaacademica_greedy(struct _malla *malla) {
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
int max_periodo(struct _malla *malla) {
	int i, saux=sbest[0];
	for(i=0;i<malla->n_periodos;i++)
	{
		if(sbest[i]>saux) {
			saux=sbest[i];
		}
	}
	return saux;
}

/*
*	Determina el periodo con menor carga académica. Si hay dos iguales se queda con el primero que encuentra.
*/
int min_periodo(int p, struct _malla *malla) {
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
void greedy(struct _malla *malla) {
	struct _cursos *aux;
	aux=primero;
	/*
	*	Elegir punto de partida.
	*/
	while(aux!=NULL) {
		if(aux->prereq==NULL) {
			aux->periodo=1;
			if (aux==getultimo(primero))
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
		cargar_cargaacademica_greedy(malla);
		while(prerequisitos_insatisfechos(aux->prereq) || aux->periodo!=0) {
			if (aux==getultimo(primero))
				aux=primero;
			else
				aux=aux->siguiente;
		}
		if(aux->prereq==NULL) {
			/*
			*	Asignar al periodo con menor cantidad de carga.
			*/
			aux->periodo=min_periodo(1, malla);
		}
		else {
			// Encontrar el periodo más lejano de sus prerequisitos, asignar entre ese y el infinito al perido con menor carga academica.
			aux->periodo=min_periodo(ultimo_periodo(aux->prereq), malla);
		}
	}
}


/*
*	Quicksort, cortesía de http://www.geeksforgeeks.org/quicksort-on-singly-linked-list/
*/
// Partitions the list taking the last element as the pivot
struct _cursos *partition(struct _cursos *head, struct _cursos *end,
                       struct _cursos **newHead, struct _cursos **newEnd)
{
    struct _cursos *pivot = end;
    struct _cursos *prev = NULL, *cur = head, *tail = pivot;
 
    // During partition, both the head and end of the list might change
    // which is updated in the newHead and newEnd variables
    while (cur != pivot)
    {
        if (cur->periodo < pivot->periodo)
        {
            // First node that has a value less than the pivot - becomes
            // the new head
            if ((*newHead) == NULL)
                (*newHead) = cur;
 
            prev = cur;  
            cur = cur->siguiente;
        }
        else // If cur node is greater than pivot
        {
            // Move cur node to next of tail, and change tail
            if (prev)
                prev->siguiente = cur->siguiente;
            struct _cursos *tmp = cur->siguiente;
            cur->siguiente = NULL;
            tail->siguiente = cur;
            tail = cur;
            cur = tmp;
        }
    }
 
    // If the pivot data is the smallest element in the current list,
    // pivot becomes the head
    if ((*newHead) == NULL)
        (*newHead) = pivot;
 
    // Update newEnd to the current last node
    (*newEnd) = tail;
 
    // Return the pivot node
    return pivot;
}
 
 
//here the sorting happens exclusive of the end node
struct _cursos *quickSortRecur(struct _cursos *head, struct _cursos *end)
{
    // base condition
    if (!head || head == end)
        return head;
 
    struct _cursos *newHead = NULL, *newEnd = NULL;
 
    // Partition the list, newHead and newEnd will be updated
    // by the partition function
    struct _cursos *pivot = partition(head, end, &newHead, &newEnd);
 
    // If pivot is the smallest element - no need to recur for
    // the left part.
    if (newHead != pivot)
    {
        // Set the node before the pivot node as NULL
        struct _cursos *tmp = newHead;
        while (tmp->siguiente != pivot)
            tmp = tmp->siguiente;
        tmp->siguiente = NULL;
 
        // Recur for the list before pivot
        newHead = quickSortRecur(newHead, tmp);
 
        // Change next of last node of the left half to pivot
        tmp = getultimo(newHead);
        tmp->siguiente =  pivot;
    }
 
    // Recur for the list after the pivot element
    pivot->siguiente = quickSortRecur(pivot->siguiente, newEnd);
 
    return newHead;
}






/*
*	Archivo Solución
*/
void archivo_solucion(struct _malla *malla, clock_t t1) {
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
			fprintf(fp, ": %d\n", sbest[i]);
			break;
		}
		while(aux->periodo==(i+1)) {
			fprintf(fp, "- ");
			fprintf(fp, "%d ", aux->pos);
			aux=aux->siguiente;
			if(aux==NULL)
				break;
		}
		fprintf(fp, ": %d\n", sbest[i]);
	}
	fprintf(fp, "\n");
	fprintf(fp, "Máxima Carga Académica %d\n",max_periodo(malla));
	fprintf(fp, "Tiempo Ejecución %f s", ((double)clock() - t1) / (double)CLOCKS_PER_SEC);
	fclose(fp);
}

/*
*	Iniciando del todo
*/
int main() {
	clock_t t1=clock();
	struct _malla *malla;
	malla = (struct _malla *) malloc (sizeof(struct _malla));
	primero = (struct _cursos *) NULL;
	leer(&malla);	
	greedy(malla);
	cargar_cargaacademica_greedy(malla);
	primero = quickSortRecur(primero, getultimo(primero));
	mostrar(malla);
	archivo_solucion(malla, t1);
	return 0;
 }