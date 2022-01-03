/*
  Adriana Marcolino de Souza, 10295047
  Caio Oliveira da Silva, 9390301
  Caroline Santos Corrêia, 9874735
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
unsigned char typedef bool;
#define TRUE  1
#define FALSE 0

int *ler_inteiros(const char *arquivo, const int n)
{
    FILE *f = fopen(arquivo, "r");

    int *inteiros = (int *) malloc(sizeof(int) * n);

    for (int i = 0; !feof(f); i++)
        fscanf(f, "%d\n", &inteiros[i]);

    fclose(f);

    return inteiros;
}

void inicia_tempo()
{
    srand(time(NULL));
    _ini = clock();
}

double finaliza_tempo()
{
    _fim = clock();
    return ((double) (_fim - _ini)) / CLOCKS_PER_SEC;
}

/*
  Única função feita por nós. Realiza uma busca e altera tanto o valor da
  variável encontrados caso encontre o valor buscado quanto muda a posição
  do valor encontrado para a primeira posição do vetor.
*/
void busca_move_para_frente(int *entradas,
						   int *encontrados,
						   const int busca,
						   const int N)
{
	int aux;

	for (int i = 0; i < N; i++) {
		if (entradas[i] == busca) {
			*encontrados += 1;
			int aux = entradas[i];
			entradas[i] = entradas[0];
			entradas[0] = aux;
			return;
		}
	}

	return;
}

int main(int argc, char const *argv[])
{
    const int N = 50000;
    unsigned encontrados = 0;

    int *entradas = ler_inteiros("inteiros_entrada.txt", N);
    int *consultas = ler_inteiros("inteiros_busca.txt", N);
	
    // realiza busca sequencia com realocação
    inicia_tempo();
    for (int i = 0; i < N; i++) {
		busca_move_para_frente(entradas, &encontrados, consultas[i], N);
    }
    double tempo_busca = finaliza_tempo();
	
	printf("Tempo de busca por item:\t%.7fs\n", tempo_busca / N);
    printf("Tempo de busca         :\t%fs\n", tempo_busca);
    printf("Itens encontrados      :\t%d\n", encontrados);

	free(entradas);
	free(consultas);
	
    return 0;
}
