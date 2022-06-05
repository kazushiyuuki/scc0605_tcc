#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// Numero de palavras reservadas
#define NUM_RESERVADAS 17
// Tamanho das strings para a saida de alguns automatos
#define MAX_TAMANHO 124

// Vetor dos operadores possiveis
extern char simb_operadores[];
// Vetor dos operadores que podem ser compostos por dois caracteres
extern char simb_operadores_especiais[];

// Armazena as palavras reservadas
char reservadas[NUM_RESERVADAS][MAX_TAMANHO];

// Funcao que decide qual automato deve ser acionado
void lexico(char *str, FILE *resultado, int linha);
// Automato de identificadores
int automatoIdentificadores(char palavra[], int i, FILE *ponteiro_saida, int linha);
// Automato de numeros
int automatoNumeros(char palavra[], int i, FILE *ponteiro_saida, int linha);
// Automato de comentarios
int comentario(char *str, int i, FILE *resultado, int linha);
// Automato de operadores
int operadores(char *str, int i, FILE *resultado);
// Funcao que verifica se um char eh operador
int eh_operador(char c);
// Funcao que inicia o vetor de palavras reservadas
void salvaReservadas();
// Funcao que identifica se um char eh um numero
int indentificaNumero(char c);