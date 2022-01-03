
#ifndef _SORTING_H_
#define _SORTING_H_


/*
  Caroline Santos Corrêia, 9874735
  Caio Oliveira da Silva, 9390301
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/*
  Número de vezes que cada algoritmo irá ordenar uma lista.
*/
#define repeat 25

typedef int elem;

/*
  A lista que será ordenada.
*/
typedef struct {
   elem *vec;
   int size;
} list;

/*
  As funções que irão criar e ordenar as listas.
*/
void create_list(list *);
void delete_list(list *);
int insert_list(list *, elem);
void print_list(list *);
int increasing_list(list *, const int);
int decreasing_list(list *, const int);
int random_list(list *, const int);

void bubblesort(list *);
void bubblesort_optimized(list *);
void quicksort(list *);
void radixsort(list *);
void heapsort(list *);

#endif