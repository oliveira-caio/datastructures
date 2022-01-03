#include "sorting.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
  Caroline Santos Corrêia, 9874735
  Caio Oliveira da Silva, 9390301
*/

void bubblesort(list *l) {
	elem aux;

	for (int i = 0; i < l->size - 1; i++) {
		for (int j = 0; j < l->size - i - 1; j++) {
			if (l->vec[j] > l->vec[j+1]) {
				aux = l->vec[j];
				l->vec[j] = l->vec[j+1];
				l->vec[j+1] = aux;
			}
		}
	}

	return;
}


void bubblesort_optimized(list *l) {
	elem aux;
	int sorted = 0;

	for (int i = 0; i < l->size - 1; i++) {
		for (int j = 0; j < l->size - i - 1; j++) {
			if (l->vec[j] > l->vec[j+1]) {
				aux = l->vec[j];
				l->vec[j] = l->vec[j+1];
				l->vec[j+1] = aux;
				sorted = 1;
			}
		}
		if (sorted == 0) return;
	}

	return;
}


int partition(list *l, int begin, int end) {
	int i = begin - 1;
	elem aux, pivot = l->vec[end];
	
	for (int j = begin; j < end; j++) {
		if (l->vec[j] < pivot) {
			i++;
			aux = l->vec[i];
			l->vec[i] = l->vec[j];
			l->vec[j] = aux;
		}
	}
	
	aux = l->vec[i+1];
	l->vec[i+1] = l->vec[end];
	l->vec[end] = aux;

	return i + 1;
}

int random_partition(list *l, int begin, int end) {
	srand(time(NULL));
	int k = begin + (rand() % (end - begin + 1));
	elem aux = l->vec[k];
	l->vec[k] = l->vec[end];
	l->vec[end] = aux;
	
	return partition(l, begin, end);
}

void quicksort_rec(list *l, const int begin, const int end) {
	int pivot;
	
	if (begin < end) {
		pivot = random_partition(l, begin, end);
		quicksort_rec(l, begin, pivot - 1);
		quicksort_rec(l, pivot + 1, end);
	}

	return;
}

void quicksort(list *l) {
	quicksort_rec(l, 0, l->size - 1);
	return;
}


void countingsort(list *l, const int size, const int position) {
	int *B = (int *) calloc(10, sizeof(int));
	int *C = (int *) calloc(size, sizeof(int));
	int key;
	
	for (int i = 0; i < size; i++) {
		key = l->vec[i] / position;
		key = key % 10;
		B[key]++;
	}

	for (int i = 1; i < 10; i++) {
		B[i] += B[i-1];
	}

	for (int i = (size - 1); i >= 0; i--) {
		key = l->vec[i] / position;
		key = key % 10;
		B[key]--;
		C[B[key]] = l->vec[i];
	}

	for (int i = 0; i < size; i++) {
		l->vec[i] = C[i];
	}

	free(B);
	free(C);
	return;
}

void radixsort(list *l) {
	elem max = l->vec[0];
	int position = 1;

	for (int i = 1; i < l->size; i++) {
		if (l->vec[i] > max) {
			max = l->vec[i];
		}
	}

	while (max / position > 0) {
		countingsort(l, l->size, position);
		position *= 10;
	}

	return;
}


void heapify(list *l, const int size, const int i) {
	int max = i, left = 2 * i + 1, right = 2 * i + 2;
	elem aux;
	
	if (left < size && l->vec[left] > l->vec[max]) {
		max = left;
	}

	if (right < size && l->vec[right] > l->vec[max]) {
		max = right;
	}

	if (max > i) {
		aux = l->vec[i];
		l->vec[i] = l->vec[max];
		l->vec[max] = aux;
		heapify(l, size, max);
	}
		
	return;
}

void heapsort(list *l) {
	elem aux;
	
	for (int i = (l->size / 2 - 1); i >= 0; i--) {
		heapify(l, l->size, i);
	}

	for (int i = (l->size - 1); i >= 1; i--) {
		aux = l->vec[0];
		l->vec[0] = l->vec[i];
		l->vec[i] = aux;
		heapify(l, i, 0);
	}
	
	return;
}