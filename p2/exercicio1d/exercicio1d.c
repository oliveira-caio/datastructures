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

void countingsort(int *entradas, const int N, const int posicao) {
	int *B = (int *) calloc(10, sizeof(int));
	int *C = (int *) calloc(N, sizeof(int));
	int chave;
	
	for (int i = 0; i < N; i++) {
		chave = entradas[i] / posicao;
		chave = chave % 10;
		B[chave]++;
	}

	for (int i = 1; i < 10; i++) {
		B[i] += B[i-1];
	}

	for (int i = (N - 1); i >= 0; i--) {
		chave = entradas[i] / posicao;
		chave = chave % 10;
		B[chave]--;
		C[B[chave]] = entradas[i];
	}

	for (int i = 0; i < N; i++) {
		entradas[i] = C[i];
	}

	free(B); free(C); return;
}

void radixsort(int *entradas, const int N) {
	int max = entradas[0], posicao = 1;

	for (int i = 1; i < N; i++) {
		if (entradas[i] > max) {
			max = entradas[i];
		}
	}
	
	while (max / posicao > 0) {
		countingsort(entradas, N, posicao);
		posicao *= 10;
	}

	return;
}

int *monta_indice_primario(const int N, const int S)
{
	int *tabela = malloc(sizeof(int) * (N / S));
	if (tabela == NULL) return NULL;

	for (int i = 0; i < (N / S); i++) {
		tabela[i] = i * S;
	}

	return tabela;
}

void busca_sequencial(int *entradas,
					  int *encontrados,
					  const int busca,
					  const int INI,
					  const int FIM)
{
	for (int i = INI; i < FIM; i++) {
		if (entradas[i] == busca) {
			*encontrados += 1;
			return;
		}
	}
	return;
}

/*
  Como o vetor está ordenado, sabemos que, se o elemento procurado for menor
  do que o primeiro elemento do vetor ou maior do que o último, ele não
  estará no vetor. Logo, não é preciso fazer uma busca nesse caso. No laço
  verificamos se o valor que queremos buscar está entre o valor armazenado
  na posição 0 e o valor armazenado na última posição do vetor de índices
  (40000 nesse caso). Se não estiver, tal valor será maior e, portanto,
  buscamos no restante do vetor de entradas, ie, da posição (N / S) - 1
  (40000 nesse caso) até a última do vetor.
 */
void busca_com_indice(int *entradas,
					 int *encontrados,
					 const int *tabela,
					 const int busca,
					 const int N,
					 const int S)
{
	if (busca < entradas[0] || busca > entradas[N - 1]) return;
	
	for (int i = 0; i + 1 < (N / S); i++) {
		if (entradas[tabela[i]] <= busca && busca < entradas[tabela[i + 1]])
			return busca_sequencial(entradas,
									encontrados,
									busca,
									tabela[i],
									tabela[i + 1]);
	}
	
	busca_sequencial(entradas, encontrados, busca, tabela[(N / S) - 1], N);
	
	return;
}

int main(int argc, char const *argv[])
{
    const int N = 50000;
    const int S = 10000;
    unsigned encontrados = 0;

    int *entradas = ler_inteiros("inteiros_entrada.txt", N);
    int *consultas = ler_inteiros("inteiros_busca.txt", N);

    // ordenar entrada
	radixsort(entradas, N);
	
    // criar tabela de indice
	int *tabela = monta_indice_primario(N, S);
	if (tabela == NULL) return -1;
	
    // realizar consultas na tabela de indices 
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        /* buscar o elemento consultas[i] na entrada */
		busca_com_indice(entradas, &encontrados, tabela, consultas[i], N, S);
    }
    double tempo_busca = finaliza_tempo();

	printf("Tempo de busca por item:\t%.7fs\n", tempo_busca / N);
    printf("Tempo de busca         :\t%fs\n", tempo_busca);
    printf("Itens encontrados      :\t%d\n", encontrados);

	free(entradas);
	free(consultas);
	
    return 0;
}
