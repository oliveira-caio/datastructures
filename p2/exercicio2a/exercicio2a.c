/*
  Adriana Marcolino de Souza, 10295047
  Caio Oliveira da Silva, 9390301
  Caroline Santos Corrêia, 9874735
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h> // funções strcmp e strcpy
#include <math.h>

// Definição das variaveis que controlam a medição de tempo
clock_t _ini, _fim;

// Definição do tipo booleano
typedef unsigned char bool;
#define TRUE  1
#define FALSE 0

// Definição do tipo string
typedef char * string;

#define MAX_STRING_LEN 20

typedef struct
{
	string *entradas;
} hash;


unsigned converter(string s)
{
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

unsigned h_div(unsigned x, unsigned i, unsigned B)
{
    return ((x % B) + i) % B;
}

unsigned h_mul(unsigned x, unsigned i, unsigned B)
{
    const double A = 0.6180;
    return ((int) ((fmod(x * A, 1) * B) + i)) % B;
}

/*
  Função criada apenas para desalocar as strings, como o nome indica.
*/
void desaloca_strings(string *s, const unsigned tam) {
	for (int i = 0; i < tam; i++)
		free(s[i]);
	free(s);
}

/*
  ------------------------------------------------------------------------------
*/

/*
  Função que inicializa a tabela hash. Essencialmente alocamos um vetor de
  strings com o tamanho dado por B, que representa o número de chaves possíveis.
*/
void inicializar(hash *t, const unsigned B)
{
	t->entradas = (string *) calloc(B, sizeof(string));
}

/*
  Função que desaloca a tabela hash.
*/
void destruir(hash *t, const unsigned B) {
	for (int i = 0; i < B; i++) {
		if (t->entradas[i] != NULL)
			free(t->entradas[i]);
	}
	free(t->entradas);
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
  seguida abrimos um laço para realizar diversas tentativas de inserção da 
  string desejada, pois pode ocorrer colisões na hora da inserção. O primeiro
  if verifica se já adicionamos a string na tabela hash. Caso não tenhamos
  feito isso, alocamos o espaço necessário para inserir a string, copiamos a
  string usando strcpy pra tabela e retornamos. O else if verifica se a string
  já foi adicionada na tabela. Caso já tenha sido, ignoraremos a string a ser
  adicionada, pois não admitiremos duplicatas, mas não estamos considerando tal
  caso como uma colisão. E, por fim, se cair no else, é porque já existe alguma
  string na posição que desejávamos adicionar, mas tal string não é repetida.
  Logo, temos uma colisão e a variável colisões é alterada, pois foi passada
  por referência.
*/
void inserir(hash *t,
			 unsigned *colisoes,
			 const string k,
			 const unsigned B,
			 unsigned (*funcao_hash) (unsigned, unsigned, unsigned))
{
	unsigned x, pos;

	x = converter(k);
	
	for (int i = 0; i < B; i++) {
		pos = funcao_hash(x, i, B);
		if (t->entradas[pos] == NULL) {
			t->entradas[pos] = (string) calloc(MAX_STRING_LEN, sizeof(char));
			strcpy(t->entradas[pos], k);
			return;
		} else if (!strcmp(t->entradas[pos], k)) {
			return;
		} else {
			*colisoes += 1;
		}
	}
	
	return;
}

/*
  Essa é a função de busca na tabela hash. Tal função é muito parecida
  com a função de inserção, mas dessa vez estamos passando a variável
  encontrados por referência para sabermos quando encontramos uma string
  e a string k agora representa a busca ao invés de uma inserção. Como
  a função é a mesma tanto pra função de hash h_div quanto h_mul, estamos
  novamente passando um ponteiro pra função como parâmetro para realizar
  o hash adequado dependendo de quando chamarmos a busca.

  A lógica é quase a mesma, o que muda são as verificações que devemos fazer.
  O primeiro if verifica se existe alguém na posição calculada para a string k
  que desejamos buscar. Se a posição for nula naquela posição, significa que
  o código hash calculado para tal string nunca foi calculado previamente e,
  portanto, tal string não foi adicionada na nossa tabela. O else if verifica
  se a string que estamos buscando existe na tabela hash. Se existir, alteramos
  a variável encontrados e saímos da função. Se não encontrarmos, recalculamos
  o código da string e verificamos novamente.
*/
void buscar(hash *t,
			unsigned *encontrados,
			const string k,
			const unsigned B,
			unsigned (*funcao_hash) (unsigned, unsigned, unsigned))
{
	unsigned x, pos;

	x = converter(k);

	for (int i = 0; i < B; i++) {
		pos = funcao_hash(x, i, B);
		if (t->entradas[pos] == NULL) {
			return;
		} else if (!strcmp(t->entradas[pos], k)) {
			*encontrados += 1;
			return;
		}
	}

	return;
}

int main(int argc, char const *argv[])
{
    unsigned N = 50000;
    unsigned M = 70000;
    unsigned B = 150001;

    unsigned colisoes_h_div = 0;
    unsigned colisoes_h_mul = 0;

    unsigned encontrados_h_div = 0;
    unsigned encontrados_h_mul = 0;

    string *insercoes = ler_strings("strings_entrada.txt", N);
    string *consultas = ler_strings("strings_busca.txt", M);

	hash t;

    // cria tabela hash com hash por divisão
	inicializar(&t, B);
	
    // inserção dos dados na tabela hash usando hash por divisão
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash
		inserir(&t, &colisoes_h_div, insercoes[i], B, h_div);
    }
    double tempo_insercao_h_div = finaliza_tempo();
	
    // consulta dos dados na tabela hash usando hash por divisão
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash
		buscar(&t, &encontrados_h_div, consultas[i], B, h_div);
    }
    double tempo_busca_h_div = finaliza_tempo();

    // limpa a tabela hash com hash por divisão
	destruir(&t, B);

	

    // cria tabela hash com hash por multiplicação
	inicializar(&t, B);
	
    // inserção dos dados na tabela hash usando hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < N; i++) {
        // inserir insercoes[i] na tabela hash
		inserir(&t, &colisoes_h_mul, insercoes[i], B, h_mul);
    }
    double tempo_insercao_h_mul = finaliza_tempo();

    // busca dos dados na tabela hash com hash por multiplicação
    inicia_tempo();
    for (int i = 0; i < M; i++) {
        // buscar consultas[i] na tabela hash
		buscar(&t, &encontrados_h_mul, consultas[i], B, h_mul);
    }
    double tempo_busca_h_mul = finaliza_tempo();

    // limpa a tabela hash com hash por multiplicação
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
