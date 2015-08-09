/*
*	Esta libreria contiene las funciones necesarias para el orden de la lista de cursos
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define MAX_LEN 100

struct _cursos *get_prev(struct _cursos *cursos, struct _cursos *node);
struct _cursos *get_post(struct _cursos *cursos, struct _cursos *node);
struct _cursos *get_last(struct _cursos *cursos);
void swap(int rd1, int rd2, struct _cursos **cursos);
void shuffle_sort(struct _cursos **cursos);
int lencursos (struct _cursos *cursos);
struct _cursos *partition_periodo_asc(struct _cursos *head, struct _cursos *end, struct _cursos **newHead, struct _cursos **newEnd);
struct _cursos *partition_periodo_des(struct _cursos *head, struct _cursos *end, struct _cursos **newHead, struct _cursos **newEnd);
struct _cursos *partition_grado_des(struct _cursos *head, struct _cursos *end, struct _cursos **newHead, struct _cursos **newEnd);
struct _cursos *partition_grado_asc(struct _cursos *head, struct _cursos *end, struct _cursos **newHead, struct _cursos **newEnd);
struct _cursos *quickSortRecur(struct _cursos *head, struct _cursos *end, int i);

/*
* Estructura de prerequisitos
*/
struct _prereq {
	int curso;
	struct _prereq *siguiente;
};

/*
* Estructura de requisitos
*/
struct _req {
	int curso;
	struct _req *siguiente;
};

/*
* Estructura de cursos
*/
struct _cursos {
	int pos;
	int creditos;
	int periodo;
	int grado;
	struct _prereq *prereq;
	struct _req *req;
	struct _cursos *siguiente;
};

/*
* Largo del listado de cursos
*/
int lencursos(struct _cursos *cursos) {
	int len=0;
	while (cursos!=NULL) {
		len++;
		cursos=cursos->siguiente;
	}
	return len;
}

/*
* Entrega el curso anterior a node, en caso de no existir entrega NULL
*/
struct _cursos *get_prev(struct _cursos *cursos, struct _cursos *node) {
	struct _cursos *prev=NULL;
	while(cursos!=NULL) {
		if(cursos->siguiente==node) {
			prev=cursos;
			break;
		}
		cursos=cursos->siguiente;
	}
	return prev;
}

/*
* Entrega el curso posterior a node, en caso de no existir entrega NULL
*/
struct _cursos *get_post(struct _cursos *cursos, struct _cursos *node) {
	struct _cursos *post=NULL;
	while(cursos!=NULL) {
		if (cursos==node) {
			post=cursos->siguiente;
			break;
		}
		cursos=cursos->siguiente;
	}
	return post;
}

/*
* Entrega el último curso del listado
*/
struct _cursos *get_last(struct _cursos *cursos) {
	while (cursos->siguiente!=NULL)
		cursos=cursos->siguiente;
	return cursos;
}

/*
* Intercambia la posición deel curso rd1 con el curso rd2
*/
void swap(int rd1, int rd2, struct _cursos **cursos) {
	struct _cursos *aux, *prev1, *prev2, *post1, *post2, *node1=NULL, *node2=NULL;
	aux=*cursos;
	while (aux!=NULL) {
		if (node1!= NULL && node2!=NULL)
			break;
		else if (aux->pos==rd1){
			node1=aux;
			if(node1==*cursos) {
				prev1=NULL;
				post1=get_post((*cursos),node1);
			}
			else if (node1==get_last(*cursos)) {
				prev1=get_prev((*cursos),node1);
				post1=NULL;
			}
			else {
				prev1=get_prev((*cursos),node1);
				post1=get_post((*cursos),node1);
			}
		}
		else if (aux->pos==rd2){
			node2=aux;
			if(node2==*cursos) {
				prev2=NULL;
				post2=get_post((*cursos),node2);
			}
			else if (node2==get_last(*cursos)) {
				prev2=get_prev((*cursos),node2);
				post2=NULL;
			}
			else {
				prev2=get_prev((*cursos),node2);
				post2=get_post((*cursos),node2);
			}
		}
		aux=aux->siguiente;
	}
	if(prev1==NULL) {
		*cursos=node2;
		if (prev2==node1 && post1==node2) {
			node2->siguiente=node1;
			node1->siguiente=post2;	
		}
		else {
			node2->siguiente=post1;
			prev2->siguiente=node1;
			node1->siguiente=post2;
		}
	}
	else if (prev2==NULL) {
		*cursos=node1;
		if (prev1==node2 && post2==node1) {
			node1->siguiente=node2;
			node2->siguiente=post1;
		}
		else {
			node1->siguiente=post2;
			prev1->siguiente=node2;
			node2->siguiente=post1;
		}
	}
	else {
		if (prev2==node1 && post1==node2) {
			prev1->siguiente=node2;
			node2->siguiente=node1;
			node1->siguiente=post2;
		}
		else if (prev1==node2 && post2==node1) {
			prev2->siguiente=node1;
			node1->siguiente=node2;
			node2->siguiente=post1;
		}
		else {
			prev1->siguiente=node2;
			node2->siguiente=post1;
			prev2->siguiente=node1;
			node1->siguiente=post2;
		}
	}
}

/*
* Realiza un orden aleatorio al listado de cursos, realiza tantas iteraciones como cursos hayan.
*/
void shuffle_sort(struct _cursos **cursos) {
	int i,rd1,rd2,lencs=lencursos(*cursos);
	srand ( time(NULL) );
	for (i=0;i<lencs;i++) {
		rd1=(rand()%lencs);
		rd2=(rand()%lencs);
		if(rd1!=rd2)
			swap(rd1,rd2,&(*cursos));
	}
}

/*
* Quicksort, cortesía de http://www.geeksforgeeks.org/quicksort-on-singly-linked-list/
* Se utiliza para dar un orden rápido al listado de cursos.
* Se tiene en variantes respecto al periodo y al grado de los cursos. Las variantes tienen su versión ascendente o descendente.
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

struct _cursos *partition_grado_des(struct _cursos *head, struct _cursos *end,
                       struct _cursos **newHead, struct _cursos **newEnd)
{
    struct _cursos *pivot = end;
    struct _cursos *prev = NULL, *cur = head, *tail = pivot;
 
    // During partition, both the head and end of the list might change
    // which is updated in the newHead and newEnd variables
    while (cur != pivot)
    {
        if (cur->grado > pivot->grado)
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

struct _cursos *partition_grado_asc(struct _cursos *head, struct _cursos *end,
                       struct _cursos **newHead, struct _cursos **newEnd)
{
    struct _cursos *pivot = end;
    struct _cursos *prev = NULL, *cur = head, *tail = pivot;
 
    // During partition, both the head and end of the list might change
    // which is updated in the newHead and newEnd variables
    while (cur != pivot)
    {
        if (cur->grado < pivot->grado)
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
    	pivot = partition_grado_des(head, end, &newHead, &newEnd);
    else if (i==2)
    	pivot = partition_grado_asc(head, end, &newHead, &newEnd);
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
        tmp = get_last(newHead);
        tmp->siguiente =  pivot;
    }
 
    // Recur for the list after the pivot element
    pivot->siguiente = quickSortRecur(pivot->siguiente, newEnd,i);
 
    return newHead;
}