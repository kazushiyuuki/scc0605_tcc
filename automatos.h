#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUM_RESERVADAS 17
#define MAX_TAMANHO 124

extern char simb_operadores[];
extern char simb_operadores_especiais[];
char reservadas[NUM_RESERVADAS][MAX_TAMANHO];

void lexico(char *str, FILE *resultado, int linha);
int automatoIdentificadores(char palavra[], int i, FILE *ponteiro_saida);
int automatoNumeros(char palavra[], int i, FILE *ponteiro_saida);
int comentario(char *str, int i, FILE *resultado, int linha);
int operadores(char *str, int i, FILE *resultado);
int eh_operador(char c);
void salvaReservadas();
int indentificaNumero(char c);