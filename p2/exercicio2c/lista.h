#include <stdlib.h>
#include <string.h>

#define MAX_STRING_LEN 20

typedef char * string;

typedef struct no
{
	string info;
	struct no *prox;
} No;

typedef struct
{
	No *ini;
} Lista;


void criar_lista(Lista *l)
{
	l->ini = NULL;
}

void destruir_lista(Lista *l)
{
	No *p = l->ini, *aux;
	
	while (p != NULL) {
		aux = p->prox;
		free(p->info);
		free(p);
		p = aux;
	}
	
	l->ini = NULL;
}

No *buscar_lista(Lista *l, string e)
{
	No *p = l->ini;
	
	while (p != NULL) {
		if (!strcmp(p->info, e)) return p;
		p = p->prox;
	}
	
	return NULL;
}

int inserir_lista(Lista *l, string e)
{
	No *p, *aux;

	// Não permitimos valores repetidos.
	if (buscar_lista(l, e) != NULL) return -1;
	
	p = (No *) malloc(sizeof(No));

	// Verifica se tudo ocorreu bem com a alocação.
	if (p == NULL) return -2;
	
	p->info = malloc(MAX_STRING_LEN);
	strcpy(p->info, e);

	aux = p;
	p->prox = l->ini;
	l->ini = aux;
	
	return 0;
}
