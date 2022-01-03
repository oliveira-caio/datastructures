#include "sorting.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

/*
  Caroline Santos Corrêia, 9874735
  Caio Oliveira da Silva, 9390301
*/

/*
  Função responsável por calcular o desvio padrão de cada iteração da
  ordenação de uma lista em relação ao tempo médio gerado pelo número
  de vezes que cada algoritmo irá executar (no nosso caso, 25 vezes).

  Parâmetros:
  - um vetor de double com o tempo que cada iteração levou para ordenar a lista
  - o tempo médio gasto em todas as iterações para que seja possível calcular
  o desvio padrão

  Retorno: o desvio padrão
*/
double std_deviation(const double *dp, const double mean) {
	double result = 0;
	
	for (int i = 0; i < repeat; i++) {
		result += (dp[i] - mean) * (dp[i] - mean);
	}

	return sqrt(result / repeat);
}


/*
  Função que calcula o tempo levado por cada algoritmo de
  ordenação para ordenar uma lista.

  A principal tarefa dessa função é chamar o algoritmo passado
  no parâmetro option para ordenar a lista e medir o tempo gasto.
  Essa função chamará o algoritmo para ordenar uma lista com entradas
  aleatórias, crescentes e decrescentes. O tamanho da lista é definido
  pelo parâmetro size. Essa função também é responsável por chamar
  a função que calcula o desvio padrão do tempo levado para ordenar
  as listas e, ao final, imprimirá os tempos e o desvio padrão em um
  arquivo passado pelo parâmetro fp_output.

  Parâmetros:
  - tamanho da lista a ser ordenada
  - qual algoritmo ordenará a lista
    - 0 para o bubble sort
	- 1 para a versão otimizada do bubble sort
	- 2 para o quick sort
	- 3 para o radix sort
	- 4 para o heapsort
  - o arquivo em que o desvio padrão e os tempos calculados
  serão impressos
  
  Retorno:
  - 0 se tudo ocorreu sem problemas
  - 1 se falhou em alocar alguma lista
  - 2 se falhou em inserir algum valor na lista (essencialmente por conta
  do realloc)
*/
int compute_time(const int size, const int option, FILE *fp_output) {
	int err = 0;
	list l_rand, l_inc, l_dec;
	double *dp_rand = (double *) malloc(repeat * sizeof(double));
	double *dp_inc = (double *) malloc(repeat * sizeof(double));
	double *dp_dec = (double *) malloc(repeat * sizeof(double));
	clock_t time_iteration_rand, time_iteration_inc, time_iteration_dec;
	clock_t accumulated_time_rand = 0, accumulated_time_inc = 0, accumulated_time_dec = 0;
	
	if (dp_rand == NULL || dp_inc == NULL || dp_dec == NULL) {
		return 1;
	}

	for (int i = 0; i < repeat; i++) {
		create_list(&l_rand);
		create_list(&l_inc);
		create_list(&l_dec);
		err = random_list(&l_rand, size);
		err = increasing_list(&l_inc, size);
		err = decreasing_list(&l_dec, size);
		
		if (err) {
			return 2;
		}
		
		switch (option) {
		case 0:
			time_iteration_rand = clock();
			bubblesort(&l_rand);
			time_iteration_rand = clock() - time_iteration_rand;

			time_iteration_inc = clock();
			bubblesort(&l_inc);
			time_iteration_inc = clock() - time_iteration_inc;

			time_iteration_dec = clock();
			bubblesort(&l_dec);
			time_iteration_dec = clock() - time_iteration_dec;
			break;
		case 1:
			time_iteration_rand = clock();
			bubblesort_optimized(&l_rand);
			time_iteration_rand = clock() - time_iteration_rand;

			time_iteration_inc = clock();
			bubblesort_optimized(&l_inc);
			time_iteration_inc = clock() - time_iteration_inc;

			time_iteration_dec = clock();
			bubblesort_optimized(&l_dec);
			time_iteration_dec = clock() - time_iteration_dec;
			break;
		case 2:
			time_iteration_rand = clock();
			quicksort(&l_rand);
			time_iteration_rand = clock() - time_iteration_rand;

			time_iteration_inc = clock();
			quicksort(&l_inc);
			time_iteration_inc = clock() - time_iteration_inc;

			time_iteration_dec = clock();
			quicksort(&l_dec);
			time_iteration_dec = clock() - time_iteration_dec;
			break;
		case 3:
			time_iteration_rand = clock();
			radixsort(&l_rand);
			time_iteration_rand = clock() - time_iteration_rand;

			time_iteration_inc = clock();
			radixsort(&l_inc);
			time_iteration_inc = clock() - time_iteration_inc;

			time_iteration_dec = clock();
			radixsort(&l_dec);
			time_iteration_dec = clock() - time_iteration_dec;
			break;
		case 4:
			time_iteration_rand = clock();
			heapsort(&l_rand);
			time_iteration_rand = clock() - time_iteration_rand;

			time_iteration_inc = clock();
			heapsort(&l_inc);
			time_iteration_inc = clock() - time_iteration_inc;

			time_iteration_dec = clock();
			heapsort(&l_dec);
			time_iteration_dec = clock() - time_iteration_dec;
			break;
		}
		
		accumulated_time_rand += time_iteration_rand;
		dp_rand[i] = time_iteration_rand;
		accumulated_time_inc += time_iteration_inc;
		dp_inc[i] = time_iteration_inc;
		accumulated_time_dec += time_iteration_dec;
		dp_dec[i] = time_iteration_dec;
		
		delete_list(&l_rand);
		delete_list(&l_inc);
		delete_list(&l_dec);
	}

	fprintf(fp_output, "Tempo gasto para ordenar uma lista aleatória: %lf s.\n",
			accumulated_time_rand / (double) repeat / CLOCKS_PER_SEC);
	fprintf(fp_output, "Desvio padrão de ordenar uma lista aleatória: %lf s.\n",
			std_deviation(dp_rand, accumulated_time_rand / (double) repeat) / CLOCKS_PER_SEC);
	fprintf(fp_output, "Tempo gasto para ordenar uma lista crescente: %lf s.\n",
			accumulated_time_inc / (double) repeat / CLOCKS_PER_SEC);
	fprintf(fp_output, "Desvio padrão de ordenar uma lista crescente: %lf s.\n",
			std_deviation(dp_inc, accumulated_time_inc / (double) repeat) / CLOCKS_PER_SEC);
	fprintf(fp_output, "Tempo gasto para ordenar uma lista decrescente: %lf s.\n",
			accumulated_time_dec / (double) repeat / CLOCKS_PER_SEC);
	fprintf(fp_output, "Desvio padrão de ordenar uma lista decrescente: %lf s.\n\n",
			std_deviation(dp_dec, accumulated_time_dec / (double) repeat) / CLOCKS_PER_SEC);

	free(dp_rand);
	free(dp_inc);
	free(dp_dec);
	
	return err;
}


/*
  Função responsável por: criar o arquivo em que tudo será impresso;
  definir os tamanhos das listas que serão ordenadas; chamar a função
  que calculará o tempo para ordenar a lista com o tamanho definido;
  verificar se houve algum problema em abrir ou fechar o arquivo.

  O laço funciona da seguinte maneira: entre os tamanhos 1.000 e 10.000,
  somaremos 500 em cada iteração, resultando em listas de tamanho 1.000,
  1.500, 2.000, 2.500, ..., 9500. De 10.000 até 100.00, somaremos 5.000
  em cada iteração, obtendo listas de tamanho 15.000, 20.000, 25.000, ...,
  95.000. Ao passar de 100.000, paramos de chamar as funções do bubble sort
  e aumentamos de 50.000 em 50.000 até chegarmos em 1.000.000.
  Depois, de 1.000.000 até 10.000.000, somaremos de 1.000.000 em 1.000.000,
  obtendo, ao final desse processo, 73 listas.

  Retorno: um inteiro que corresponde a um possível erro durante o processo.
  - 0, se tudo ocorreu como planejado
  - 1, se não conseguiu alocar alguma lista
  - 2, se não conseguiu inserir um elemento em alguma lista
  - 3, se não conseguiu abrir o arquivo
  - caso contrário, retorna o erro gerado ao fechar o arquivo (caso o arquivo
  seja fechado corretamente, a função fclose retorna 0)
*/
int caller() {
	int size = 0, err = 0;
	FILE *fp_output = fopen("sorting.txt", "w+");
	
	if (fp_output == NULL) {
		return 3;
	}

	for (int j = 0; j < 73; j++) {
		if (j <= 18) {
			size = 1000 + j * 500;

			fprintf(fp_output, "Método: Bubble sort.\n");
			fprintf(fp_output, "Tamanho da lista: %d.\n", size);
			err = compute_time(size, 0, fp_output);
			if (err == 1) {
				return 1;
			} else if (err == 2) {
				return 2;
			}

			fprintf(fp_output, "Método: Bubble sort otimizado.\n");
			fprintf(fp_output, "Tamanho da lista: %d.\n", size);
			err = compute_time(size, 1, fp_output);
			if (err == 1) {
				return 1;
			} else if (err == 2) {
				return 2;
			}

			fprintf(fp_output, "Método: Quick sort.\n");
			fprintf(fp_output, "Tamanho da lista: %d.\n", size);
			err = compute_time(size, 2, fp_output);
			if (err == 1) {
				return 1;
			} else if (err == 2) {
				return 2;
			}

			fprintf(fp_output, "Método: Radix sort.\n");
			fprintf(fp_output, "Tamanho da lista: %d.\n", size);
			err = compute_time(size, 3, fp_output);
			if (err == 1) {
				return 1;
			} else if (err == 2) {
				return 2;
			}

			fprintf(fp_output, "Método: Heap sort.\n");
			fprintf(fp_output, "Tamanho da lista: %d.\n", size);
			err = compute_time(size, 4, fp_output);
			if (err == 1) {
				return 1;
			} else if (err == 2) {
				return 2;
			}

			fprintf(fp_output, "-----------------------------------\
-------------------------\n\n");
		} else if (18 < j && j <= 36) {
			size = 10000 + (j - 18) * 5000;

			fprintf(fp_output, "Método: Bubble sort.\n");
			fprintf(fp_output, "Tamanho da lista: %d.\n", size);
			err = compute_time(size, 0, fp_output);
			if (err == 1) {
				return 1;
			} else if (err == 2) {
				return 2;
			}

			fprintf(fp_output, "Método: Bubble sort otimizado.\n");
			fprintf(fp_output, "Tamanho da lista: %d.\n", size);
			err = compute_time(size, 1, fp_output);
			if (err == 1) {
				return 1;
			} else if (err == 2) {
				return 2;
			}

			fprintf(fp_output, "Método: Quick sort.\n");
			fprintf(fp_output, "Tamanho da lista: %d.\n", size);
			err = compute_time(size, 2, fp_output);
			if (err == 1) {
				return 1;
			} else if (err == 2) {
				return 2;
			}

			fprintf(fp_output, "Método: Radix sort.\n");
			fprintf(fp_output, "Tamanho da lista: %d.\n", size);
			err = compute_time(size, 3, fp_output);
			if (err == 1) {
				return 1;
			} else if (err == 2) {
				return 2;
			}

			fprintf(fp_output, "Método: Heap sort.\n");
			fprintf(fp_output, "Tamanho da lista: %d.\n", size);
			err = compute_time(size, 4, fp_output);
			if (err == 1) {
				return 1;
			} else if (err == 2) {
				return 2;
			}

			fprintf(fp_output, "-----------------------------------\
-------------------------\n\n");
		} else if (36 < j && j <= 54) {
			size = 100000 + (j - 36) * 50000;

			fprintf(fp_output, "Método: Quick sort.\n");
			fprintf(fp_output, "Tamanho da lista: %d.\n", size);
			err = compute_time(size, 2, fp_output);
			if (err == 1) {
				return 1;
			} else if (err == 2) {
				return 2;
			}

			fprintf(fp_output, "Método: Radix sort.\n");
			fprintf(fp_output, "Tamanho da lista: %d.\n", size);
			err = compute_time(size, 3, fp_output);
			if (err == 1) {
				return 1;
			} else if (err == 2) {
				return 2;
			}

			fprintf(fp_output, "Método: Heap sort.\n");
			fprintf(fp_output, "Tamanho da lista: %d.\n", size);
			err = compute_time(size, 4, fp_output);
			if (err == 1) {
				return 1;
			} else if (err == 2) {
				return 2;
			}

			fprintf(fp_output, "-----------------------------------\
-------------------------\n\n");
		} else {
			size = 1000000 + (j - 54) * 500000;

			fprintf(fp_output, "Método: Quick sort.\n");
			fprintf(fp_output, "Tamanho da lista: %d.\n", size);
			err = compute_time(size, 2, fp_output);
			if (err == 1) {
				return 1;
			} else if (err == 2) {
				return 2;
			}

			fprintf(fp_output, "Método: Radix sort.\n");
			fprintf(fp_output, "Tamanho da lista: %d.\n", size);
			err = compute_time(size, 3, fp_output);
			if (err == 1) {
				return 1;
			} else if (err == 2) {
				return 2;
			}

			fprintf(fp_output, "Método: Heap sort.\n");
			fprintf(fp_output, "Tamanho da lista: %d.\n", size);
			err = compute_time(size, 4, fp_output);
			if (err == 1) {
				return 1;
			} else if (err == 2) {
				return 2;
			}

			fprintf(fp_output, "-----------------------------------\
-------------------------\n\n");
		}
	}

	return fclose(fp_output);
}

/*
  A função main é responsável apenas por chamar a função "caller"
  e verificar se algum erro aconteceu durante a execução do programa.
*/
int main(int argc, char* argv[]) {
	int err = 0;

	printf("Se tudo ocorrer bem, o resultado estará no arquivo sorting.txt.\n");
	
	err = caller();

	if (err == 1) {
		printf("Falhou em alocar uma lista.");
		return err;
	} else if (err == 2) {
		printf("Falhou ao inserir um elemento na lista.");
		return err;
	} else if (err == 3) {
		printf("Não foi possível criar o arquivo.");
		return err;
	} else if (err) {
		printf("Erro ao fechar o arquivo.");
		return err;
	} else {
		printf("Tudo ocorreu bem.");
		return err;
	}
}