#include "sorting.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

/*
  Caroline Santos Corrêia, 9874735
  Caio Oliveira da Silva, 9390301
*/

void create_list(list *l) {
	l->size = 0;
	l->vec = NULL;
	
	return;
}

void delete_list(list *l) {
	l->size = 0;
	free(l->vec);

	return;
}

int insert_list(list *l, elem e) {
	l->vec = (elem *) realloc(l->vec, (l->size + 1) * sizeof(elem));
	if (l->vec == NULL) {
		return 1;
	}
	l->vec[l->size] = e;
	l->size++;

	return 0;
}

void print_list(list *l) {
	for (int i = 0; i < l->size; i++) {
		printf("%d ", l->vec[i]);
	}
	printf("\n");
}

/*
  Função que cria uma lista com o tamanho fornecido pelo parâmetro
  size e com entradas crescentes. A linha srand(time(NULL)) é responsável
  por gerar uma nova semente para o algoritmo que gera números aleatórios
  toda vez que essa função é chamada, fazendo com que não gere listas
  repetidas.

  A lógica para criar uma lista crescente é definir a primeira entrada
  da lista com valor 0 e em seguida colocar cada elemento com o
  valor do elemento anterior mais um número aleatório entre 0 e 100,
  o que garante que a lista de fato será crescente.

  Parâmetros:
  - a lista na qual inseriremos os elementos
  - o tamanho desejado que a lista tenha após a função executar

  Retorno:
  - 0, se tudo ocorreu bem
  - 1, se falhou em inserir algum elemento na lista (por causa do realloc)
*/
int increasing_list(list *l, const int size) {
	int err = 0;
	srand(time(NULL));

	err = insert_list(l, 0);
	if (err) return err;
	for (int i = 1; i < size; i++) {
		err = insert_list(l, (l->vec[i-1] + rand() % 100));
		if (err) return err;
	}

	return err;
}

/*
  Função que cria uma lista com o tamanho fornecido pelo parâmetro
  size e com entradas decrescentes. A linha srand(time(NULL)) é responsável
  por gerar uma nova semente para o algoritmo que gera números aleatórios
  toda vez que essa função é chamada, fazendo com que não gere listas
  repetidas.
  
  A lógica para criar uma lista decrescente é colocar o maior valor possível
  para uma variável do tipo signed int como primeiro elemento da lista e em
  seguida colocar cada elemento com o valor do anterior menos um número
  aleatório entre 0 e 100, o que garante que a lista é de fato decrescente e
  não terá valores negativos.

  Parâmetros:
  - a lista na qual inseriremos os elementos
  - o tamanho desejado que a lista tenha após a função executar

  Retorno:
  - 0, se tudo ocorreu bem
  - 1, se falhou em inserir algum elemento na lista (por causa do realloc)
*/
int decreasing_list(list *l, const int size) {
	int err = 0;
	srand(time(NULL));

	err = insert_list(l, 2147483647); // largest value allowed for a signed int (2^31 - 1)
	if (err) return err;
	for (int i = 1; i < size; i++) {
		err = insert_list(l, l->vec[i-1] - rand() % 100);
		if (err) return err;
	}
	

	return err;
}

/*
  Função que cria uma lista com o tamanho fornecido pelo parâmetro
  size e com entradas aleatórias. A linha srand(time(NULL)) é responsável
  por gerar uma nova semente para o algoritmo que gera números aleatórios
  toda vez que essa função é chamada, fazendo com que não gere listas
  repetidas.

  A lógica é simplesmente usar a função rand para gerar um
  número aleatório entre 0 e o tamanho da lista.

  Parâmetros:
  - a lista na qual inseriremos os elementos
  - o tamanho desejado que a lista tenha após a função executar

  Retorno:
  - 0, se tudo ocorreu bem
  - 1, se falhou em inserir algum elemento na lista (por causa do realloc)
*/
int random_list(list *l, const int size) {
	int err = 0;
	srand(time(NULL));
	
	for (int i = 0; i < size; i++) {
		err = insert_list(l, rand() % size);
		if (err) return err;
	}
	
	return err;
}