#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define Instancia "instancias/bacp10.txt"
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
	float carga_optima;
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

struct _cursos *primero;

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
		printf("%d\n", aux->cadena);
		aux=aux->siguiente;
	}
	printf("\n Cursos: %d\n Periodos: %d\n Min_Creditos: %d\n Max_Creditos: %d\n Min_Cursos: %d\n Max_Cursos: %d\n Prerequisitos: %d\n", malla->n_cursos, malla->n_periodos, malla->min_creditos, malla->max_creditos, malla->min_cursos, malla->max_cursos, malla->n_prerequisitos);
}

/*
*	Determina la maxima carga académica. Si hay dos iguales se queda con el primero que encuentra.
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
int det_max_periodo(struct _malla *malla,int carga_periodo[]) {
	int i, max=0, saux=carga_periodo[0];
	for(i=0;i<malla->n_periodos;i++)
	{
		if(carga_periodo[i]>saux) {
			saux=carga_periodo[i];
			max=i;
		}
	}
	return max;
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
*	Algoritmo Greedy, construye la solución inicial.
*	Función de Evaluaciación: Obtener el periodo con mayor carga académica tras asignar la minima carga posible a todos los periodos.
*	Punto de Partida: Primer curso sin prerequisitos en primer periodo.
*	Función Miope: Añadir un curso, luego añadir los cursos que lo requieren y que tengan sus prerequisitos satisfechos.
*			a su prerequisito más cercano y el último periodo.
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
			if (aux==getultimo(primero))
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
	
		//if (aux==getultimo(primero))
				aux=primero;
			//else
			//	aux=aux->siguiente;
	}
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

void push(int curso, int periodo, struct _LS **LS) {
	struct _LS *nuevo;
	nuevo = (struct _LS *) malloc (sizeof(struct _LS));
	nuevo->curso=curso;
	nuevo->periodo=periodo;
	nuevo->siguiente=*LS;
	*LS = nuevo;
}

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

int infactible(struct _malla *malla, int carga_periodo[], int cursos_periodo[], int creditos, struct _cursos *aux, int nuevo_periodo) {
	int ult=0;
	struct _prereq *prereq=aux->prereq;
	struct _req *req=aux->req;
	if (!cumple_max(malla, carga_periodo, cursos_periodo, creditos, ult) || cumple_min(malla, carga_periodo, cursos_periodo, ult) ||
		cumple_prereq(prereq,nuevo_periodo) || cumple_req(req,nuevo_periodo))
		return 1;
	return 0;
}

int check_tabulist(struct _LS *LS, int curso, int periodo) {
	while(LS!=NULL) {
		if(LS->curso==curso && LS->periodo==periodo)
			return 0;
		LS=LS->siguiente;
	}
	return 1;
}

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
void cadena(struct _cursos *aux) {
	while(aux!=NULL) {
		aux->cadena=altura(aux);
		aux=aux->siguiente;
	}
}

/*
*	Quicksort, cortesía de http://www.geeksforgeeks.org/quicksort-on-singly-linked-list/
*/
// Partitions the list taking the last element as the pivot
struct _cursos *partition_periodo_asc(struct _cursos *head, struct _cursos *end,
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

struct _cursos *partition_periodo_des(struct _cursos *head, struct _cursos *end,
                       struct _cursos **newHead, struct _cursos **newEnd)
{
    struct _cursos *pivot = end;
    struct _cursos *prev = NULL, *cur = head, *tail = pivot;
 
    // During partition, both the head and end of the list might change
    // which is updated in the newHead and newEnd variables
    while (cur != pivot)
    {
        if (cur->periodo > pivot->periodo)
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
 
/*
*	Quicksort, cortesía de http://www.geeksforgeeks.org/quicksort-on-singly-linked-list/
*/
// Partitions the list taking the last element as the pivot
struct _cursos *partition_cadena_des(struct _cursos *head, struct _cursos *end,
                       struct _cursos **newHead, struct _cursos **newEnd)
{
    struct _cursos *pivot = end;
    struct _cursos *prev = NULL, *cur = head, *tail = pivot;
 
    // During partition, both the head and end of the list might change
    // which is updated in the newHead and newEnd variables
    while (cur != pivot)
    {
        if (cur->cadena > pivot->cadena)
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

struct _cursos *partition_cadena_asc(struct _cursos *head, struct _cursos *end,
                       struct _cursos **newHead, struct _cursos **newEnd)
{
    struct _cursos *pivot = end;
    struct _cursos *prev = NULL, *cur = head, *tail = pivot;
 
    // During partition, both the head and end of the list might change
    // which is updated in the newHead and newEnd variables
    while (cur != pivot)
    {
        if (cur->cadena < pivot->cadena)
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
struct _cursos *quickSortRecur(struct _cursos *head, struct _cursos *end, int i)
{
	struct _cursos *pivot;
    // base condition
    if (!head || head == end)
        return head;
 
    struct _cursos *newHead = NULL, *newEnd = NULL;
 
    // Partition the list, newHead and newEnd will be updated
    // by the partition function
    if (i==1)
    	pivot = partition_cadena_des(head, end, &newHead, &newEnd);
    else if (i==2)
    	pivot = partition_cadena_asc(head, end, &newHead, &newEnd);
    else if (i==3)
    	pivot = partition_periodo_des(head, end, &newHead, &newEnd);
    else
    	pivot = partition_periodo_asc(head, end, &newHead, &newEnd);
 
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
        newHead = quickSortRecur(newHead, tmp,i);
 
        // Change next of last node of the left half to pivot
        tmp = getultimo(newHead);
        tmp->siguiente =  pivot;
    }
 
    // Recur for the list after the pivot element
    pivot->siguiente = quickSortRecur(pivot->siguiente, newEnd,i);
 
    return newHead;
}


/*
*	Archivo Solución
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
*	Iniciando del todo
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
	/* Medir cadena de dependencia */
	cadena(primero);
	/* Orden descendente por cadena de dependencia */
	primero = quickSortRecur(primero, getultimo(primero),1);
	/* greedy */
	greedy(malla,carga_periodo,cursos_periodo);
	/* Orden descendente por cadena de dependencia */
	//primero = quickSortRecur(primero, getultimo(primero),1);
	/* Orden ascendente por cadena de dependencia */
	//primero = quickSortRecur(primero, getultimo(primero),2);
	/* Orden descendente por periodo */
	//primero = quickSortRecur(primero, getultimo(primero),3);
	/* Orden ascendente por periodo */
	primero = quickSortRecur(primero, getultimo(primero),4);
	/* TabuSearch */
	tabusearch(malla,primero,&LS,carga_periodo,cursos_periodo);
	/* Ordenar ascendente por periodo */
	primero = quickSortRecur(primero, getultimo(primero),4);
	mostrar(malla);
	/* Crear archivo con el resultado */
	archivo_solucion(malla, t1,carga_periodo);
	return 0;
 }