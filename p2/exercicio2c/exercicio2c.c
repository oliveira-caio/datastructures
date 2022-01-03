/*
  Adriana Marcolino de Souza, 10295047
  Caio Oliveira da Silva, 9390301
  Caroline Santos Corrêia, 9874735
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>
#include "lista.h"

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
typedef unsigned char bool;
#define TRUE  1
#define FALSE 0

typedef struct {
	Lista *vet;
} hash;


unsigned converter(string s) {
   unsigned h = 0;
   for (int i = 0; s[i] != '\0'; i++) 
      h = h * 256 + s[i];
   return h;
}

string *ler_strings(const char *arquivo, const int n)
{
    FILE *f = fopen(arquivo, "r");
    
    string *strings = (string *) malloc(sizeof(string) * n);
	
    for (int i = 0; !feof(f); i++) {
        strings[i] = (string) malloc(sizeof(char) * MAX_STRING_LEN);
        fscanf(f, "%s\n", strings[i]);
    }

    fclose(f);

    return strings;
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

unsigned h_div(unsigned x, unsigned B)
{
    return x % B;
}

unsigned h_mul(unsigned x, unsigned B)
{
    const double A = 0.6180;
    return fmod(x * A, 1) * B;
}

void desaloca_strings(string *s, const unsigned tam) {
	for (int i = 0; i < tam; i++) {
		free(s[i]);
	}
	free(s);
}

/*
  ------------------------------------------------------------------------------
*/

/*
  Função que inicializa a tabela hash. Essencialmente alocamos um vetor de
  tamanho B de listas, sendo que cada nó das listas guardará a string a ser
  armazenada. O laço simplesmente cria a lista, o que significa que começaremos
  apontando pra NULL.
*/
void inicializar(hash *t, const unsigned B)
{
	t->vet = (Lista *) malloc(B * sizeof(Lista));
	
	for (int i = 0; i < B; i++)
		criar_lista(&(t->vet[i]));
}

/*
  Função que desaloca a tabela hash chamando a função que desaloca as listas.
*/
void destruir(hash *t, const unsigned B)
{
	for (int i = 0; i < B; i++)
		destruir_lista(&(t->vet[i]));
	free(t->vet);
}

/*
  Função para inserir na tabela hash. Os parâmetros são a própria tabela,
  passada por referência para alterarmos o valor dela de fato dentro da função;
  a variável de colisões também passada por referência pela mesma razão, que,
  obviamente, representará quantas vezes duas strings caíram na mesma chave;
  a string que desejamos inserir na tabela hash, que denotamos por k; o tamanho
  da tabela hash, que denotamos por B; e um ponteiro pra função que chamará
  a função h_div ou a h_div dependendo de quando a inserção for feita.

  A lógica da inserção é a seguinte: primeiramente convertemos a string para
  um valor numérico usando a função converter que foi dada no template. Em
  seguida verificamos se já adicionamos a string na tabela hash. Se já tivermos
  adicionado, significa que o início da lista não aponta mais para NULL e temos
  uma colisão. Logo, incrementamos a variável de colisão, pois a passamos por
  referência, e em seguida adicionamos na lista. A inserção na lista é feita
  no início, o que economiza tempo de inserção, mas pode perder na busca.
  O retorno da função é um inteiro, pois pode ocorrer problema na hora de
  alocar o espaço para armazenar a string.
*/
int inserir(hash *t,
			 unsigned *colisoes,
			 const string k,
			 const unsigned B,
			 unsigned (*funcao_hash) (unsigned, unsigned))
{
	unsigned x, pos;

	x = converter(k);
	pos = funcao_hash(x, B);
	if (t->vet[pos].ini != NULL) *colisoes += 1;
	return inserir_lista(&(t->vet[pos]), k);	
}

/*
  Essa é a função de busca na tabela hash. Tal função é muito parecida
  com a função de inserção, mas dessa vez estamos passando a variável
  encontrados por referência para sabermos quando encontramos uma string
  e a string k agora representa a busca ao invés de uma inserção. Como
  a função é a mesma tanto pra função de hash h_div quanto h_mul, estamos
  novamente passando um ponteiro pra função como parâmetro para realizar
  o hash adequado dependendo de quando chamarmos a busca.

  A lógica é quase a mesma, o que muda é a verificação feita. O if verifica se
  existe alguém na posição calculada para a string k que desejamos buscar E,
  caso tenha alguém, ie, caso o vetor seja não nulo, buscamos a string na lista
  chamando a função do TAD. Note que tal verificação faz sentido, pois como
  o condicional é o &&, então a segunda condição só é verificada se a primeira
  passar, o que impede de acessarmos ponteiro nulo. Por fim, se encontrar a
  string desejada, incrementamos a variável encontrados e retornamos.
*/
void buscar(hash *t,
			unsigned *encontrados,
			const string k,
			const unsigned B,
			unsigned (*funcao_hash) (unsigned, unsigned))
{
	unsigned x, pos;

	x = converter(k);
	
	pos = funcao_hash(x, B);
	if (t->vet[pos].ini != NULL && buscar_lista(&(t->vet[pos]), k) != NULL)
		*encontrados += 1;
	
	return;
}

int main(int argc, char const *argv[])
{
    const int N = 50000;
    const int M = 70000;
    const int B = 150001;

    unsigned colisoes_h_div = 0;
    unsigned colisoes_h_mul = 0;

    unsigned encontrados_h_div = 0;
    unsigned encontrados_h_mul = 0;

    string *insercoes = ler_strings("strings_entrada.txt", N);
    string *consultas = ler_strings("strings_busca.txt", M);
    
	hash t;
	
    // cria tabela hash com hash por divisão
	inicializar(&t, B);
	
    // inserção dos dados na tabela hash com hash por divisão
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash
		if (inserir(&t, &colisoes_h_div, insercoes[i], B, h_div) == -2)
			return -2;
    }
    double tempo_insercao_h_div = finaliza_tempo();

    // busca dos dados na tabela hash com hash por divisão
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash
		// se o resultado for -2, significa que houve problema de alocação
		buscar(&t, &encontrados_h_div, consultas[i], B, h_div);
    }
    double tempo_busca_h_div = finaliza_tempo();

    // destroi tabela hash com hash por divisão
	destruir(&t, B);



    // cria tabela hash com hash por multiplicação
	inicializar(&t, B);
	
    // inserção dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash
		// se o resultado for -2, significa que houve problema de alocação
		if (inserir(&t, &colisoes_h_mul, insercoes[i], B, h_mul) == -2)
			return -2;
    }
    double tempo_insercao_h_mul = finaliza_tempo();

    // busca dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash
		buscar(&t, &encontrados_h_mul, consultas[i], B, h_mul);
    }
    double tempo_busca_h_mul = finaliza_tempo();

    // destroi tabela hash com hash por multiplicação
	destruir(&t, B);


    printf("Hash por Divisão\n");
    printf("Colisões na inserção                 : %d\n", colisoes_h_div);
	printf("Tempo de inserção de cada string     : %.7fs\n",
		   tempo_insercao_h_div / N);
    printf("Tempo de inserção de todas as strings: %fs\n",
		   tempo_insercao_h_div);
	printf("Tempo de busca de cada string        : %.7fs\n",
		   tempo_busca_h_div / M);
    printf("Tempo de busca de todas as strings   : %fs\n", tempo_busca_h_div);
    printf("Itens encontrados                    : %d\n", encontrados_h_div);
    printf("\n");
    printf("Hash por Multiplicação\n");
    printf("Colisões na inserção                 : %d\n", colisoes_h_mul);
	printf("Tempo de inserção de cada string     : %.7fs\n",
		   tempo_insercao_h_mul / N);
    printf("Tempo de inserção de todas as strings: %fs\n",
		   tempo_insercao_h_mul);
	printf("Tempo de busca de cada string        : %.7fs\n",
		   tempo_busca_h_mul / M);
    printf("Tempo de busca de todas as strings   : %fs\n", tempo_busca_h_mul);
    printf("Itens encontrados                    : %d\n", encontrados_h_mul);

	desaloca_strings(insercoes, N);
	desaloca_strings(consultas, M);
	
    return 0;
}
